/*
 * th128.cpp
 *
 *  Created on: 2010-9-10
 *      Author: Argon
 */

#include "common.h"

void th128decode(char *file, unsigned char * buffer, unsigned int flength) {
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
	decode(rawdata, length, 0x800, 0x5e, 0xe7);
	decode(rawdata, length, 0x80, 0x7d, 0x36);
	rlength = decompress(rawdata, decodedata, length);
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(decodedata, dlength, 1, fpraw);
	fclose(fpraw);
	printf("Decompression done.\n");

	droprate = *(float*) (&decodedata[0x54]);
	stage = decodedata[0x58];
	route = decodedata[0x5c];
	rank = decodedata[0x64];
	stagedata = &decodedata[0x70];
	final = *(unsigned int*) (&decodedata[0x14]);
	for (i = 1; i < stage; ++i) {
		stagedata = stagedata + 0x90 + *((unsigned int*) &stagedata[0x8]);
		score[i - 1] = *((unsigned int*) &stagedata[0xc]);
	}
	score[stage - 1] = final;
	stagedata = &decodedata[0x70];

	FILE * fprec = fopen(frec, "wb");
	fprintf(fprec, "file : %s\r\n", file);
	fprintf(fprec, "rawdata size = 0x%x\r\n", length);
	fprintf(fprec, "alloced size = 0x%x\r\n", dlength);
	fprintf(fprec, "decoded size = 0x%x\r\n", rlength);
	fprintf(fprec, "----------\r\n");
	fprintf(fprec, "player : %s\r\n", decodedata);
	fprintf(fprec, "route : ");
	if (route == 0)
		fprintf(fprec, "A1");
	else if (route == 1)
		fprintf(fprec, "A2");
	else if (route == 2)
		fprintf(fprec, "B1");
	else if (route == 3)
		fprintf(fprec, "B2");
	else if (route == 4)
		fprintf(fprec, "C1");
	else if (route == 5)
		fprintf(fprec, "C2");
	else if (route == 6)
		fprintf(fprec, "Extra");
	fprintf(fprec, "\r\n");
	fprintf(fprec, "score : %d0\r\n", final);
	fprintf(fprec, "rank :");
	if (rank == 0)
		fprintf(fprec, "Easy");
	else if (rank == 1)
		fprintf(fprec, "Normal");
	else if (rank == 2)
		fprintf(fprec, "Hard");
	else if (rank == 3)
		fprintf(fprec, "Lunatic");
	else if (rank == 4)
		fprintf(fprec, "Extra");
	fprintf(fprec, "\r\n");
	fprintf(fprec, "slow rate : %f%%\r\n", droprate);
	fprintf(fprec, "----------\r\n");

	for (i = 0; i < stage; ++i) {
		replaydata = stagedata + 0x90;
		frame = *((unsigned int*) &stagedata[0x4]);
		llength = *((unsigned int*) &stagedata[0x8]);
		fpsdata = replaydata + frame * 6;
		shootlevel = stagedata[0x10] + 1;
		perfectfreeze = *((unsigned int*) &stagedata[0x84]) / 100;
		lf = *((unsigned int*) &stagedata[0x80]) / 100;
		freezearea = *((float*) &stagedata[0x88]);
		if (rank != 0x4)
			fprintf(fprec, "stage : %d \r\n", i + 1);
		else
			fprintf(fprec, "stage : extra \r\n");
		fprintf(fprec, "score = %d0 | frame size  = 0x%x | stage length = 0x%x | ", score[i], frame, llength);
		fprintf(fprec, "life: %d%% | level: = %d | perfect freeze : %d | freeze area : %f%%\r\n", lf, shootlevel, perfectfreeze, freezearea);
		fprintf(fprec, "= Time =   ====================================================== ");
		fprintf(fprec, "Operations ======================================================\r\n");
		skey[60] = 0;
		for (j = 0; j < frame; ++j) {
			if (!(j % 60))
				fprintf(fprec, "[%06d]   ", j / 60);
			framekey = (replaydata[j * 6] >> 4) & 0xf;
			skey[j % 60] = keys[framekey];
			if (!((j + 1) % 60))
				fprintf(fprec, "%s\r\n", (char *) skey);
		}
		if (j % 60) {
			skey[j % 60] = 0;
			fprintf(fprec, "%s\r\n", (char *) skey);
		}
		stagedata += llength + 0x90;
	}
	fclose(fprec);
	delete[] decodedata;
}
