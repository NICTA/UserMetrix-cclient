/*
 *  UserMetrix.c
 *  UserMetrix-cclient
 *
 *  Copyright 2011 UserMetrix. All rights reserved.
 */
#include <curl/curl.h>
#include "UserMetrix.h"

int UM_init(unsigned int newProjectID, const char *newTmpDirectory) {
	FILE *uuid_fp;
	char start_time[UM_TIME_STR_LEN];
	time_t raw_time;
	struct tm *time_info;

	UM_instance.projectID = newProjectID;	
	
	/* Store the log path. */
	UM_instance.logFile = malloc(strlen(newTmpDirectory) + strlen("usermetrix.log") + 1);
	if (UM_instance.logFile == NULL) {
		return -1;
	}
	strcpy(UM_instance.logFile, newTmpDirectory);
	strcat(UM_instance.logFile, "usermetrix.log");
	
	/* Store the UUID file path. */
	UM_instance.uuidFile = malloc(strlen(newTmpDirectory) + strlen("usermetrix.id") + 1);
	if (UM_instance.uuidFile == NULL) {
		free(UM_instance.uuidFile);
		return -1;
	}
	strcpy(UM_instance.uuidFile, newTmpDirectory);
	strcat(UM_instance.uuidFile, "usermetrix.id");

	/* See if a UUID exists for this user. */
	uuid_fp = fopen(UM_instance.uuidFile, "r");
	if (uuid_fp == NULL) {
		/* No UUID - generate one and save it to disk. */
		uuid_fp = fopen(UM_instance.uuidFile, "w");
		if (uuid_fp == NULL) {
			free(UM_instance.logFile);
			free(UM_instance.uuidFile);
			
			return -1;
		}
		genUUID(UM_instance.uuid);
		fprintf(uuid_fp, "%s\n", UM_instance.uuid);
		
	} else {
		/* UUID file exists - open it and fetch the uuid. */
		fscanf(uuid_fp, "%35s", UM_instance.uuid);
	}
	fclose(uuid_fp);

	/* Check for the existance of a partial log. */
	UM_instance.tmpLog = fopen(UM_instance.logFile, "r+");
	if (UM_instance.tmpLog != NULL) {
		/* Incomplete log present - close log and send. */
		fseek(UM_instance.tmpLog, 0, SEEK_END);
		fprintf(UM_instance.tmpLog, "duration: -1\n");
		fclose(UM_instance.tmpLog);
		UM_sendlog();
	}	

	/* Open a fresh project log for this instance. */
	UM_instance.tmpLog = fopen(UM_instance.logFile, "w");
	if (UM_instance.tmpLog == NULL) {
		free(UM_instance.logFile);
		free(UM_instance.uuidFile);
		return -1;
	}

	/* Initalise the log: */
	fprintf(UM_instance.tmpLog, "---\n");
	fprintf(UM_instance.tmpLog, "v: %i\n", UM_LOG_VERSION);
	fprintf(UM_instance.tmpLog, "system:\n");
	fprintf(UM_instance.tmpLog, "  id: %s\n", UM_instance.uuid);
	fprintf(UM_instance.tmpLog, "  os: %s\n", UM_PLATFORM);

	/* Determine the start time for this usage log. */
	time(&raw_time);
	time_info = localtime(&raw_time);
	strftime(start_time, UM_TIME_STR_LEN, "%Y-%m-%dT%H:%M:%S.000+1000", time_info);
	fprintf(UM_instance.tmpLog, "  start: %s\n", start_time);

	/* Mark the start of the log. */
	fprintf(UM_instance.tmpLog, "meta:\n");
	fprintf(UM_instance.tmpLog, "log:\n");

	return 0;
}

void UM_writeMessageDetails(const char *message, const char *source) {
	if (UM_instance.tmpLog != NULL) {
		unsigned int time = (unsigned int) (clock() / (double) CLOCKS_PER_SEC * 1000.0);
		fprintf(UM_instance.tmpLog, "    time: %i\n", time);
		fprintf(UM_instance.tmpLog, "    source: %s\n", source);
		fprintf(UM_instance.tmpLog, "    message: \"%s\"\n", message);
		
	}
}

void UM_setCanSendLogs(UserMetrix_bool canSend) {
	UM_instance.canSendLogs = canSend;
}

void UM_view(const char *message, const char *source) {
	if (UM_instance.tmpLog != NULL) {
		fprintf(UM_instance.tmpLog, "  - type: view\n");
		UM_writeMessageDetails(message, source);
	}
}

void UM_event(const char *message, const char *source) {
	if (UM_instance.tmpLog != NULL) {
		fprintf(UM_instance.tmpLog, "  - type: usage\n");
		UM_writeMessageDetails(message, source);
	}
}

void UM_frustration(const char *message, const char *source) {
	if (UM_instance.tmpLog != NULL) {
		fprintf(UM_instance.tmpLog, "  - type: frustration\n");
		UM_writeMessageDetails(message, source);
	}
}

void UM_error(const char *message, const char *source) {
	if (UM_instance.tmpLog != NULL) {
		fprintf(UM_instance.tmpLog, "  - type: error\n");
		UM_writeMessageDetails(message, source);
	}
}

void UM_sendlog() {
	CURL *curl;
	CURLM *multi_handle;
	int still_running;
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char buf[] = "Expect:";
	char URL[256] = {0};

	if (UM_instance.canSendLogs == UM_false) {
		/* Not permitted to send logs - leave method. */
		return;
	}

	curl_formadd(&formpost,
				 &lastptr,
				 CURLFORM_COPYNAME, "upload",
				 CURLFORM_FILE, UM_instance.logFile,
				 CURLFORM_END);

	/* Send data to usermetrix. */
	curl = curl_easy_init();
	multi_handle = curl_multi_init();
	headerlist = curl_slist_append(headerlist, buf);
	
	if (curl && multi_handle) {
		sprintf(URL, "http://usermetrix.com/projects/%i/log", UM_instance.projectID);
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		
		curl_multi_add_handle(multi_handle, curl);
		curl_multi_perform(multi_handle, &still_running);
		
		while (still_running) {
			struct timeval timeout;
			int rc; /* select() return code */ 
			
			fd_set fdread;
			fd_set fdwrite;
			fd_set fdexcep;
			int maxfd = -1;
			
			long curl_timeo = -1;
			FD_ZERO(&fdread);
			FD_ZERO(&fdwrite);
			FD_ZERO(&fdexcep);
			
			/* set a suitable timeout to play around with */ 
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			curl_multi_timeout(multi_handle, &curl_timeo);
			if(curl_timeo >= 0) {
				timeout.tv_sec = curl_timeo / 1000;
				if(timeout.tv_sec > 1)
					timeout.tv_sec = 1;
				else
					timeout.tv_usec = (curl_timeo % 1000) * 1000;
			}
			
			/* get file descriptors from the transfers */ 
			curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
			
			/* In a real-world program you OF COURSE check the return code of the
			 function calls.  On success, the value of maxfd is guaranteed to be
			 greater or equal than -1.  We call select(maxfd + 1, ...), specially in
			 case of (maxfd == -1), we call select(0, ...), which is basically equal
			 to sleep. */ 
			
			rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);

			switch(rc) {
				case -1:
					/* select error */ 
					break;
				case 0:
				default:
					/* timeout or readable/writable sockets */ 
					curl_multi_perform(multi_handle, &still_running);
					break;
			}
		}
		
		curl_multi_cleanup(multi_handle);
		
		/* always cleanup */ 
		curl_easy_cleanup(curl);
		
		/* then cleanup the formpost chain */ 
		curl_formfree(formpost);
		
		/* free slist */ 
		curl_slist_free_all (headerlist);

		/* Remove the file that we successfully sent. */
		remove(UM_instance.logFile);
	}
}

void UM_shutdown() {
	/* Append the duration tag to the file before closing. */
	if (UM_instance.tmpLog != NULL) {
		unsigned int time = (unsigned int) (clock() / (double) CLOCKS_PER_SEC * 1000.0);
		fprintf(UM_instance.tmpLog, "duration: %i\n", time);
	}
		
	/* Close the log. */
	fclose(UM_instance.tmpLog);

	/* Send the log to the UserMetrix server. */
	UM_sendlog();

	if (UM_instance.logFile != NULL) {
		free(UM_instance.logFile);
	}
	
	if (UM_instance.uuidFile != NULL) {
		free(UM_instance.uuidFile);
	}
}
