/*
 * threp.cpp
 *
 *  Created on: 2010-1-3
 *      Author: Argon.Sun
 *      Version: 1.2.8
 */

#include "common.h"

int main(int argc, char *argv[]) {
	unsigned int flength;
	unsigned char *buffer = new unsigned char[0x100000];
	if (argc < 2) {
		printf("Usage : %s [filename]", argv[0]);
		return 0;
	}
	char * file = argv[1];
	FILE * fp;
	fp = fopen(file, "rb");
	if (!fp) {
		printf("%s not found.\n", file);
		return 0;
	}
	fread(buffer, 0x100000, 1, fp);
	flength = _filelength(fp->_file);
	fclose(fp);
	if (*(int *) buffer == 0x50523654) { //"T6RP"
		th06decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x50523754) { //"T7RP"
		th07decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x50523854) { //"T8RP"
		th08decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x72303174) { //"t10r"
		th10decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x72313174) { //"t11r"
		th11decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x72323174) { //"t12r"
		th12decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x35323174) { //"t125"
		th125decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x72383231) { //"128r"
		th128decode(file, buffer, flength);
	} else if (*(int *) buffer == 0x72333174) { //"t13r"
		th13decode(file, buffer, flength);
	} else {
		printf("not supported format.\n");
		return 0;
	}
	delete[] buffer;
	return 0;
}
