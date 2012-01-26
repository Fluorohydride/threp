/*
 * common.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

unsigned int get_bit(unsigned char* buffer, unsigned int &pointer, unsigned char &filter, unsigned char length) {
	unsigned char i;
	unsigned int result = 0;
	unsigned char current;
	current = buffer[pointer];
	for (i = 0; i < length; ++i) {
		result <<= 1;
		if (current & filter)
			result |= 0x1;
		filter >>= 1;
		if (filter == 0) {
			pointer++;
			current = buffer[pointer];
			filter = 0x80;
		}
	}
	return result;
}
unsigned int decompress(unsigned char * buffer, unsigned char * decode, unsigned int length) {
	unsigned int pointer = 0, dest = 0, index, bits, i;
	unsigned char filter = 0x80;
	unsigned char dict[0x2010];
	memset(dict, 0, 0x2010);
	while (pointer < length) {
		bits = get_bit(buffer, pointer, filter, 1);
		if (pointer >= length)
			return dest;
		if (bits) {
			bits = get_bit(buffer, pointer, filter, 8);
			if (pointer >= length)
				return dest;
			decode[dest] = (unsigned char) bits;
			dict[dest & 0x1fff] = (unsigned char) bits;
			dest++;
		} else {
			bits = get_bit(buffer, pointer, filter, 13);
			if (pointer >= length)
				return dest;
			index = bits - 1;
			bits = get_bit(buffer, pointer, filter, 4);
			if (pointer >= length)
				return dest;
			bits += 3;
			for (i = 0; i < bits; ++i) {
				dict[dest & 0x1fff] = dict[index + i];
				decode[dest] = dict[index + i];
				dest++;
			}
		}
	}
	return dest;
}

void decode(unsigned char * buffer, int length, int block_size, unsigned char base, unsigned char add) {
	unsigned char * tbuf = new unsigned char[length];
	memcpy(tbuf, buffer, length);
	int i, p = 0, tp1, tp2, hf, left = length;
	if ((left % block_size) < (block_size / 4))
		left -= left % block_size;
	left -= length & 1;
	while (left) {
		if (left < block_size)
			block_size = left;
		tp1 = p + block_size - 1;
		tp2 = p + block_size - 2;
		hf = (block_size + (block_size & 0x1)) / 2;
		for (i = 0; i < hf; ++i, ++p) {
			buffer[tp1] = tbuf[p] ^ base;
			base += add;
			tp1 -= 2;
		}
		hf = block_size / 2;
		for (i = 0; i < hf; ++i, ++p) {
			buffer[tp2] = tbuf[p] ^ base;
			base += add;
			tp2 -= 2;
		}
		left -= block_size;
	}
	delete[] tbuf;
}
