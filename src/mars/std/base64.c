#include "base64.h"
#include <stdio.h>

const unsigned char _B64_ENC_TABLE[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

const unsigned char _B64_ENC_TABLE_URL[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '-', '_'
};

uint32_t _B64_DEC_TABLE[256] = {
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  62U, 63U, 62U, 62U, 63U, 
	52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U, 61U, 0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  1U,  2U,  3U,  4U,  5U,  6U,  7U,  8U,  9U,  10U, 11U, 12U, 13U, 14U, 
	15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U, 0U,  0U,  0U,  0U,  63U,
	0U,  26U, 27U, 28U, 29U, 30U, 31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U, 40U, 
	41U, 42U, 43U, 44U, 45U, 46U, 47U, 48U, 49U, 50U, 51U, 0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U, 
	0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U,  0U
};

/*
Modified implementation by Jouni Malinen: http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c
*/
size_t base64Encode(unsigned char* data, size_t len, char* result, size_t maxCount, bool urlSafe) {
	const unsigned char* base64Table = &_B64_ENC_TABLE[0];
	if (urlSafe) { base64Table = &_B64_ENC_TABLE_URL[0]; }

	// Error check
	if (!data) { return 0; }

	// Calculate output buffer size
	size_t outLen = (4U * ((len + 2U) / 3U)) + 1U;
	if (!result) { return outLen; }
	else if (outLen > maxCount) { return 0; }

	// Iterate through data stream
	unsigned char* end = data + len;
	unsigned char* in = data;
	unsigned char* out = result;
	while(end - in >= 3) {
		*out++ = base64Table[in[0] >> 2];
		*out++ = base64Table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*out++ = base64Table[((in[1] & 0x0F) << 2) | (in[2] >> 6)];
		*out++ = base64Table[in[2] & 0x3F];
		in += 3;
	}
	if ((end - in) > 0) {
		*out++ = base64Table[in[0] >> 2];
		if (end - in == 1) {
			*out++ = base64Table[(in[0] & 0x03) << 4];
			*out++ = '=';
		}
		else {
			*out++ = base64Table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
			*out++ = base64Table[(in[1] & 0x0F) << 2];
		}
		*out++ = '=';
	}
	*out++ = '\0';
	return outLen;
}

/*
Modified implementation by polfosol: https://stackoverflow.com/a/37109258
*/
size_t base64Decode(char* data, unsigned char* result, size_t maxCount) {
	uint32_t* base64Table = &_B64_DEC_TABLE[0];

	// Error check
	if (!data) { return 0; }
	size_t len = strlen(data);
	if (len == 0) { return 0; }

	// Calculate output size
	size_t pad1 = (len % 4) || (data[len - 1] == '=');
	size_t pad2 = pad1 && ((len % 4 > 2) || data[len - 2] != '=');
	size_t strLen = ((len - pad1) / 4) << 2;
	size_t outLen = ((strLen / 4U) * 3U) + pad1 + pad2;
	if (!result) { return outLen; }
	else if (outLen > maxCount) { return 0; }

	// Iterate through string
	size_t i = 0, j = 0;
	for (; i < strLen; i += 4) {
		uint32_t n = base64Table[data[i]] << 18 | base64Table[data[i + 1]] << 12 | base64Table[data[i + 2]] << 6 | base64Table[data[i + 3]];
		result[j++] = (unsigned char)(n >> 16);
		result[j++] = (unsigned char)(n >> 8 & 0xFF);
		result[j++] = (unsigned char)(n & 0xFF);
	}
	if (pad1) {
		uint32_t n = base64Table[data[strLen]] << 18 | base64Table[data[strLen + 1]] << 12;
		result[j++] = (unsigned char)(n >> 16);
		if (pad2) {
			n |= base64Table[data[strLen + 2]] << 6;
			result[j++] = (unsigned char)((n >> 8) & 0xFF);
		}
	}
	return outLen;
}