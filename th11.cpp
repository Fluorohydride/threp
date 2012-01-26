/*
 * th11.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

void th11decode(char *file, unsigned char * buffer, unsigned int flength) {
	char frec[50];
	char fraw[50];
	unsigned int score[6], maxp[6], graze[6];
	unsigned int frame, rlength, llength, i, l;
	unsigned char character, ctype, rank, clear;
	unsigned char framekey, stage;
	unsigned char *rawdata = &buffer[0x24], *stagedata, *replaydata, *fpsdata, *decodedata;
	unsigned int length = *((unsigned int*) (&buffer[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buffer[0x20]));
	short keys[] = { 0xf0a1, 0xfca1, 0xfda1, 0xfda1, 0xfba1, 0x49a8, 0x4ca8, 0x49a8, 0xfaa1, 0x4aa8, 0x4ba8, 0x4aa8, 0xfba1,
			0x49a8, 0x4ca8, 0x49a8 };
	short skey[61];
	sprintf(frec, "%s.txt", file);
	sprintf(fraw, "%s.raw", file);

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0xaa, 0xe1);
	decode(rawdata, length, 0x40, 0x3d, 0x7a);
	rlength = decompress(rawdata, decodedata, length);
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(decodedata, dlength, 1, fpraw);
	fclose(fpraw);
	printf("Decompression done.");

	stagedata = &decodedata[0x70];
	stage = decodedata[0x58];
	if (stage > 6)
		stage = 6;
	graze[0] = 0;
	for (i = 1; i < stage; ++i) {
		stagedata = stagedata + 0x90 + *((unsigned int*) &stagedata[0x8]);
		score[i - 1] = *((unsigned int*) &stagedata[0xc]);
		maxp[i] = *((unsigned int*) &stagedata[0x14]);
		graze[i] = *((unsigned int*) &stagedata[0x34]);
	}
	score[stage - 1] = *((unsigned int*) &decodedata[0x14]);
	character = decodedata[0x5c];
	ctype = decodedata[0x60];
	rank = decodedata[0x64];
	clear = decodedata[0x68];
	if (rank == 0)
		maxp[0] = 25000;
	else if (rank == 1)
		maxp[0] = 50000;
	else if (rank == 2)
		maxp[0] = 50000;
	else
		maxp[0] = 200000;

	FILE * fprec = fopen(frec, "wb");
	fprintf(fprec, "file : %s\r\n", file);
	fprintf(fprec, "rawdata size = 0x%x\r\n", length);
	fprintf(fprec, "alloced size = 0x%x\r\n", dlength);
	fprintf(fprec, "decodedatad size = 0x%x\r\n", rlength);

	decodedata[8] = 0;
	fprintf(fprec, "----------\r\n");
	fprintf(fprec, "player : %s\r\n", decodedata);
	fprintf(fprec, "character : ");
	if (!character)
		fprintf(fprec, "Reimu");
	else
		fprintf(fprec, "Marisa");
	if (ctype == 0)
		fprintf(fprec, "A\r\n");
	else if (ctype == 1)
		fprintf(fprec, "B\r\n");
	else
		fprintf(fprec, "C\r\n");
	fprintf(fprec, "rank : ");
	if (rank == 0)
		fprintf(fprec, "easy\r\n");
	else if (rank == 1)
		fprintf(fprec, "normal\r\n");
	else if (rank == 2)
		fprintf(fprec, "hard\r\n");
	else if (rank == 3)
		fprintf(fprec, "lunatic\r\n");
	else
		fprintf(fprec, "extra\r\n");
	fprintf(fprec, "clear : ");
	if (clear == 8)
		fprintf(fprec, "all\r\n");
	else if (clear == 7)
		fprintf(fprec, "extra\r\n");
	else
		fprintf(fprec, "%d\r\n", clear);
	fprintf(fprec, "----------\r\n");

	stagedata = &decodedata[0x70];
	for (l = 0; l < stage; ++l) {
		replaydata = stagedata + 0x90;
		frame = *((unsigned int*) &stagedata[0x4]);
		llength = *((unsigned int*) &stagedata[0x8]);
		fpsdata = replaydata + frame * 6;
		if (rank != 0x4)
			fprintf(fprec, "stage : %d ", stagedata[0]);
		else
			fprintf(fprec, "stage : extra ");
		fprintf(fprec, "score = %d0 | frame size  = 0x%x | stage length = 0x%x\r\n", score[l], frame, llength);
		fprintf(fprec, "= Time =   ====================================================== ");
		fprintf(fprec, "Operations ======================================================\r\n");
		skey[60] = 0;
		for (i = 0; i < frame; ++i) {
			if (!(i % 60))
				fprintf(fprec, "[%06d]   ", i / 60);
			framekey = (replaydata[i * 6] >> 4) & 0xf;
			skey[i % 60] = keys[framekey];
			if (!((i + 1) % 60))
				fprintf(fprec, "%s\r\n", (char *) skey);
		}
		if (i % 60) {
			skey[i % 60] = 0;
			fprintf(fprec, "%s\r\n", (char *) skey);
		}
		stagedata += llength + 0x90;
	}
	fclose(fprec);
	delete[] decodedata;
}
