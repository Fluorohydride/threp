/*
 * th07.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

void th07decode(char *file, unsigned char * buffer, unsigned int flength) {
	char frec[50];
	char fraw[50];
	unsigned char *rawdata = &buffer[0x54], *decodedata;
	unsigned int i, length, dlength, rlength, checksum;
	unsigned char base;
	base = *((unsigned char*) (&buffer[0x0d]));
	sprintf(frec, "%s.txt", file);
	sprintf(fraw, "%s.raw", file);
	for (i = 0x10; i < flength; ++i) {
		buffer[i] -= base;
		base += 7;
	}
	checksum = 0x3f000318;
	for (i = 0x0d; i < flength; ++i)
		checksum += buffer[i];
	length = *((unsigned int*) (&buffer[0x14]));
	dlength = *((unsigned int*) (&buffer[0x18]));
	decodedata = new unsigned char[dlength];
	rlength = decompress(rawdata, decodedata, length);
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(decodedata, rlength, 1, fpraw);
	fclose(fpraw);
	printf("Decompression done.");

	delete[] decodedata;
}
