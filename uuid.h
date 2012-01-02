/*
 * uuid.h
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