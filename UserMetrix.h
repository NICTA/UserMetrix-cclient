/*
 *  UserMetrix.c
 *  UserMetrix-cclient
 *
 *  Copyright 2011 UserMetrix. All rights reserved.
 */
#ifndef _USERMETRIX_CCLIENT_	
#define _USERMETRIX_CCLIENT_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UM_LOG_VERSION 1
#define UM_TIME_STR_LEN 29

#ifndef UM_PLATFORM
#ifdef _WIN32
#define UM_PLATFORM "MS Windows"
#elif __APPLE__
#define UM_PLATFORM "Apple OSX"
#else
#define UM_PLATFORM "unknown"
#endif
#endif

typedef enum {
	UM_false = 0,
	UM_true = 1
} UserMetrix_bool;

typedef struct {
	/* The ID of the project on the UserMetrix server. */
	unsigned int projectID;

	/* The file to store log content. */
	char *logFile;
	
	/* The file that stores the users UUID. */
	char *uuidFile;
	
	/* The UUID for this user. */
	char uuid[UM_UUID_LEN];
	
	/* Has the user permitted transmission of anonymous usage and error logs. */
	UserMetrix_bool canSendLogs;

	/* The log file that we are dumping content too. */
	FILE *tmpLog;
	
} UserMetrix;

/* The instance that holds all the UserMetrix related information. */
UserMetrix UM_instance;

/**
 * Initalise the UserMetrix log - call this when you start your application.
 *
 * @param newProjectID Your UserMetrix project ID - this is displayed at startup.
 * @param newTempDirectory The location you wish to store temporary files.
 */
int UM_init(unsigned int newProjectID, const char *newTmpDirectory);

/**
 * Has the user granted permission to send usage and error logs to the UserMetrix server?
 * Before setting this to true, you should ask the user to opt-in to the service.
 *
 * @param canSend true if we are permitted to send logs to the UserMetrix Server, false otherwise.
 */
void UM_setCanSendLogs(UserMetrix_bool canSend);

/**
 * Append a view tag to your log - a view is where a new screen / form / etc is presented to the user, i.e. the 'V' in a MVC framework.
 *
 * @param message The name of the view being presented to the user.
 * @param source The source of the message, automatically detected if __FILE__ is supplied.
 */
void UM_view(const char *message, const char *source);

/**
 * Append a event tag to your log - an event is an action invoked by the user i.e. 'save', or 'open'.
 *
 * @param message The name of the event being invoked by the user.
 * @param source The source of the message, automatically detected if __FILE__ is supplied.
 */
void UM_event(const char *message, const char *source);

/**
 * Append a frustration tag to your log.
 *
 * @param message the nature of the frustration encountered by the user.
 * @param source The source of the message, automatically detected if __FILE__ is supplied.
 */
void UM_frustration(const char *message, const char *source);

/**
 * Append an error tag to your log - an error is where the software has trapped a malfunction (i.e. error code / exception).
 *
 * @param message The message describing the error that has been trapped.
 * @param source The source of the message, automatically detected if __FILE__ is supplied.
 */
void UM_error(const char *message, const char *source);

/**
 * Shutdown the UserMetrix log - call this when your application gracefully exits.
 */
void UM_shutdown();
	
/**
 * Packages and sends the UserMetrix log to the central server.
 */
void UM_sendlog();	
	
#ifdef __cplusplus
}
#endif

#endif
