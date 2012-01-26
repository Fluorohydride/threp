/*
 * th13.cpp
 *
 *  Created on: 2011-4-16
 *      Author: Argon
 */

#include "common.h"

void th13decode(char *file, unsigned char * buffer, unsigned int flength) {
	char frec[50];
	char fraw[50];
	unsigned int score[3], final;
	float droprate, freezearea;
	unsigned int frame, rlength, llength, perfectfreeze, lf, i, j;
	unsigned char route, rank, clear, shootlevel;
	unsigned char framekey, stage;
	unsigned char *rawdata = &buffer[0x24], *stagedata, *replaydata, *fpsdata, *decodedata;
	unsigned int length = *((unsigned int*) (&buffer[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buffer[0x20]));
	unsigned short keys[16] =
			{ 0xf0a1, 0xfca1, 0xfda1, 0xfda1, 0xfba1, 0x49a8, 0x4ca8, 0x49a8, 0xfaa1, 0x4aa8, 0x4ba8, 0x4aa8, 0xfba1, 0x49a8, 0x4ca8, 0x49a8 };
	unsigned short skey[61];
	sprintf(frec, "%s.txt", file);
	sprintf(fraw, "%s.raw", file);

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x400, 0x5c, 0xe1);
	decode(rawdata, length, 0x100, 0x7d, 0x3a);
	rlength = decompress(rawdata, decodedata, length);
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(decodedata, dlength, 1, fpraw);
	fclose(fpraw);
	printf("Decompression done.\n");

	delete[] decodedata;
}
