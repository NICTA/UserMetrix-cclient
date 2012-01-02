/*
 * uuid.c
 * UserMetrix-cclient
 *
 * Copyright (c) 2012 UserMetrix Pty Ltd. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
