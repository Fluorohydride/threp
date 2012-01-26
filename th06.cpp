/*
 * th06.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

void th06decode(char *file, unsigned char * buffer, unsigned int flength) {
	char frec[50];
	char fraw[50];
	unsigned int i, checksum;
	unsigned char base;
	base = *((unsigned char*) (&buffer[0x0e]));
	sprintf(frec, "%s.txt", file);
	sprintf(fraw, "%s.raw", file);
	for (i = 0x0f; i < flength; ++i) {
		buffer[i] -= base;
		base += 7;
	}
	checksum = 0x3f000318;
	for (i = 0x0e; i < flength; ++i)
		checksum += buffer[i];
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(buffer, flength, 1, fpraw);
	fclose(fpraw);
	printf("Decoding done.");
}
