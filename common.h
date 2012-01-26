/*
 * common.h
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void th06decode(char *file, unsigned char * buffer, unsigned int flength);
void th07decode(char *file, unsigned char * buffer, unsigned int flength);
void th08decode(char *file, unsigned char * buffer, unsigned int flength);
void th10decode(char *file, unsigned char * buffer, unsigned int flength);
void th11decode(char *file, unsigned char * buffer, unsigned int flength);
void th12decode(char *file, unsigned char * buffer, unsigned int flength);
void th125decode(char *file, unsigned char * buffer, unsigned int flength);
void th128decode(char *file, unsigned char * buffer, unsigned int flength);
void th13decode(char *file, unsigned char * buffer, unsigned int flength);

unsigned int get_bit(unsigned char* buffer, unsigned int &pointer, unsigned char &filter, unsigned char length);
unsigned int decompress(unsigned char * buffer, unsigned char * decode, unsigned int length);
void decode(unsigned char * buffer, int length, int block_size, unsigned char base, unsigned char add);


#endif /* COMMON_H_ */
