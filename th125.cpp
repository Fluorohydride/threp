/*
 * th125.cpp
 *
 *  Created on: 2010-9-10
 *      Author: Argon
 */

#include "common.h"

void th125decode(char *file, unsigned char * buffer, unsigned int flength) {
	char frec[50];
	char fraw[50];
	unsigned char *rawdata = &buffer[0x24],*decodedata;
	unsigned int rlength;
	unsigned int length = *((unsigned int*) (&buffer[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buffer[0x20]));
	sprintf(frec, "%s.txt", file);
	sprintf(fraw, "%s.raw", file);

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0x5e, 0xe1);
	decode(rawdata, length, 0x40, 0x7d, 0x3a);
	rlength = decompress(rawdata, decodedata, length);
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(decodedata, dlength, 1, fpraw);
	fclose(fpraw);
	printf("Decompression done.");
	delete[] decodedata;
}
