/*
 *  uuid.h
 *  UserMetrix-cclient
 *
 *  Copyright 2011 UserMetrix. All rights reserved.
 */
#ifndef _UUID_
#define _UUID_

#define UM_UUID_CHUNK_LEN 9
#define UM_UUID_LEN 36
#define UM_UUID_FACTOR 1000000000.0

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generate a partial chunk for a UUID, typically chunks consist of either 4 or 8 characters.
 *
 * @param chunk The buffer to store the resulting chunk.
 * @param length The length of the chunk to generate, should be less than UM_UUID_CHUNK_LEN.
 */
void genUUIDChunk(char chunk[UM_UUID_CHUNK_LEN], int length);

/**
 * Generate a new random universally unique identifier for the user of this softare.
 *
 * @param uuid The resuting hexadecimal UUID in the form XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
 *
 */
void genUUID(char uuid[UM_UUID_LEN]);

#ifdef __cplusplus
}
#endif
	
#endif