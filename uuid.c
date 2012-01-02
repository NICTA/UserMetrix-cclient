/*
 *  uuid.c
 *  UserMetrix-cclient
 *
 *  Copyright 2011 UserMetrix. All rights reserved.
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "uuid.h"

void genUUIDChunk(char chunk[UM_UUID_CHUNK_LEN], int length) {
	double t = ((double) rand() / (double) (RAND_MAX));	
	sprintf(chunk, "%0x", (int) (t * UM_UUID_FACTOR));
	if (length < (UM_UUID_CHUNK_LEN - 1)) {
		chunk[length] = '\0';
	}	
}

void genUUID(char uuid[UM_UUID_LEN]) {
	char chunk1[UM_UUID_CHUNK_LEN];
	char chunk2[UM_UUID_CHUNK_LEN];
	char chunk3[UM_UUID_CHUNK_LEN];
	char chunk4[UM_UUID_CHUNK_LEN];
	char chunk5[UM_UUID_CHUNK_LEN];
	char chunk6[UM_UUID_CHUNK_LEN];
	
	// Seed the random number generator.
	srand((unsigned int) time(NULL));		
	
	genUUIDChunk(chunk1, 8);
	genUUIDChunk(chunk2, 4);
	genUUIDChunk(chunk3, 4);
	genUUIDChunk(chunk4, 4);
	genUUIDChunk(chunk5, 4);
	genUUIDChunk(chunk6, 8);
	
	sprintf(uuid, "%s-%s-%s-%s-%s%s", chunk1, chunk2, chunk3, chunk4, chunk5, chunk6);	
}
