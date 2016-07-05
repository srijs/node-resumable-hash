#include <stdlib.h>
#include <memory.h>
#include "sha1.hpp"

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))

static const uint32_t init[SHA1_STATE_SIZE] = {
	0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xc3d2e1f0
};

Sha1::Sha1(uint8_t data[SHA1_STATE_SIZE]) {
	if (data == nullptr) {
		this->ctx.init(init);
	} else {
		this->ctx.deserialize(data);
	}
}

void Sha1::transform() {
	uint32_t a, b, c, d, e;
	uint32_t schedule[16];
	uint32_t temp;

	#define SCHEDULE(i)  \
		temp = schedule[(i - 3) & 0xF] ^ schedule[(i - 8) & 0xF] ^ schedule[(i - 14) & 0xF] ^ schedule[(i - 16) & 0xF];  \
		schedule[i & 0xF] = temp << 1 | temp >> 31;

	#define LOADSCHEDULE(i)  \
		schedule[i] =                           \
			  (uint32_t)(this->ctx.data)[i * 4 + 0] << 24  \
			| (uint32_t)(this->ctx.data)[i * 4 + 1] << 16  \
			| (uint32_t)(this->ctx.data)[i * 4 + 2] <<  8  \
			| (uint32_t)(this->ctx.data)[i * 4 + 3];

	#define ROUND0a(a, b, c, d, e, i)  LOADSCHEDULE(i)  ROUNDTAIL(a, b, e, ((b & c) | (~b & d))         , i, 0x5A827999)
	#define ROUND0b(a, b, c, d, e, i)  SCHEDULE(i)      ROUNDTAIL(a, b, e, ((b & c) | (~b & d))         , i, 0x5A827999)
	#define ROUND1(a, b, c, d, e, i)   SCHEDULE(i)      ROUNDTAIL(a, b, e, (b ^ c ^ d)                  , i, 0x6ED9EBA1)
	#define ROUND2(a, b, c, d, e, i)   SCHEDULE(i)      ROUNDTAIL(a, b, e, ((b & c) ^ (b & d) ^ (c & d)), i, 0x8F1BBCDC)
	#define ROUND3(a, b, c, d, e, i)   SCHEDULE(i)      ROUNDTAIL(a, b, e, (b ^ c ^ d)                  , i, 0xCA62C1D6)

	#define ROUNDTAIL(a, b, e, f, i, k)  \
		e += (a << 5 | a >> 27) + f + UINT32_C(k) + schedule[i & 0xF];  \
		b = b << 30 | b >> 2;

	a = this->ctx.state[0];
	b = this->ctx.state[1];
	c = this->ctx.state[2];
	d = this->ctx.state[3];
	e = this->ctx.state[4];

	ROUND0a(a, b, c, d, e,  0)
	ROUND0a(e, a, b, c, d,  1)
	ROUND0a(d, e, a, b, c,  2)
	ROUND0a(c, d, e, a, b,  3)
	ROUND0a(b, c, d, e, a,  4)
	ROUND0a(a, b, c, d, e,  5)
	ROUND0a(e, a, b, c, d,  6)
	ROUND0a(d, e, a, b, c,  7)
	ROUND0a(c, d, e, a, b,  8)
	ROUND0a(b, c, d, e, a,  9)
	ROUND0a(a, b, c, d, e, 10)
	ROUND0a(e, a, b, c, d, 11)
	ROUND0a(d, e, a, b, c, 12)
	ROUND0a(c, d, e, a, b, 13)
	ROUND0a(b, c, d, e, a, 14)
	ROUND0a(a, b, c, d, e, 15)
	ROUND0b(e, a, b, c, d, 16)
	ROUND0b(d, e, a, b, c, 17)
	ROUND0b(c, d, e, a, b, 18)
	ROUND0b(b, c, d, e, a, 19)
	ROUND1(a, b, c, d, e, 20)
	ROUND1(e, a, b, c, d, 21)
	ROUND1(d, e, a, b, c, 22)
	ROUND1(c, d, e, a, b, 23)
	ROUND1(b, c, d, e, a, 24)
	ROUND1(a, b, c, d, e, 25)
	ROUND1(e, a, b, c, d, 26)
	ROUND1(d, e, a, b, c, 27)
	ROUND1(c, d, e, a, b, 28)
	ROUND1(b, c, d, e, a, 29)
	ROUND1(a, b, c, d, e, 30)
	ROUND1(e, a, b, c, d, 31)
	ROUND1(d, e, a, b, c, 32)
	ROUND1(c, d, e, a, b, 33)
	ROUND1(b, c, d, e, a, 34)
	ROUND1(a, b, c, d, e, 35)
	ROUND1(e, a, b, c, d, 36)
	ROUND1(d, e, a, b, c, 37)
	ROUND1(c, d, e, a, b, 38)
	ROUND1(b, c, d, e, a, 39)
	ROUND2(a, b, c, d, e, 40)
	ROUND2(e, a, b, c, d, 41)
	ROUND2(d, e, a, b, c, 42)
	ROUND2(c, d, e, a, b, 43)
	ROUND2(b, c, d, e, a, 44)
	ROUND2(a, b, c, d, e, 45)
	ROUND2(e, a, b, c, d, 46)
	ROUND2(d, e, a, b, c, 47)
	ROUND2(c, d, e, a, b, 48)
	ROUND2(b, c, d, e, a, 49)
	ROUND2(a, b, c, d, e, 50)
	ROUND2(e, a, b, c, d, 51)
	ROUND2(d, e, a, b, c, 52)
	ROUND2(c, d, e, a, b, 53)
	ROUND2(b, c, d, e, a, 54)
	ROUND2(a, b, c, d, e, 55)
	ROUND2(e, a, b, c, d, 56)
	ROUND2(d, e, a, b, c, 57)
	ROUND2(c, d, e, a, b, 58)
	ROUND2(b, c, d, e, a, 59)
	ROUND3(a, b, c, d, e, 60)
	ROUND3(e, a, b, c, d, 61)
	ROUND3(d, e, a, b, c, 62)
	ROUND3(c, d, e, a, b, 63)
	ROUND3(b, c, d, e, a, 64)
	ROUND3(a, b, c, d, e, 65)
	ROUND3(e, a, b, c, d, 66)
	ROUND3(d, e, a, b, c, 67)
	ROUND3(c, d, e, a, b, 68)
	ROUND3(b, c, d, e, a, 69)
	ROUND3(a, b, c, d, e, 70)
	ROUND3(e, a, b, c, d, 71)
	ROUND3(d, e, a, b, c, 72)
	ROUND3(c, d, e, a, b, 73)
	ROUND3(b, c, d, e, a, 74)
	ROUND3(a, b, c, d, e, 75)
	ROUND3(e, a, b, c, d, 76)
	ROUND3(d, e, a, b, c, 77)
	ROUND3(c, d, e, a, b, 78)
	ROUND3(b, c, d, e, a, 79)

	this->ctx.state[0] += a;
	this->ctx.state[1] += b;
	this->ctx.state[2] += c;
	this->ctx.state[3] += d;
	this->ctx.state[4] += e;
}

void Sha1::update(uint8_t data[], size_t len) {
	size_t i;

	for (i = 0; i < len; ++i) {
		this->ctx.data[this->ctx.datalen] = data[i];
		this->ctx.datalen++;
		if (this->ctx.datalen == 64) {
			this->transform();
			this->ctx.bitlen += 512;
			this->ctx.datalen = 0;
		}
	}
}

uint8_t *Sha1::finalize(uint32_t *lenptr) {
	*lenptr = SHA1_BLOCK_SIZE;
	uint8_t *hash = (uint8_t *)malloc(SHA1_BLOCK_SIZE);

	uint32_t i = this->ctx.datalen;

	// Pad whatever data is left in the buffer.
	if (this->ctx.datalen < 56) {
		this->ctx.data[i++] = 0x80;
		while (i < 56) {
			this->ctx.data[i++] = 0x00;
		}
	}
	else {
		this->ctx.data[i++] = 0x80;
		while (i < 64) {
			this->ctx.data[i++] = 0x00;
		}
		this->transform();
		memset(this->ctx.data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	this->ctx.bitlen += this->ctx.datalen * 8;
	this->ctx.data[63] = this->ctx.bitlen;
	this->ctx.data[62] = this->ctx.bitlen >> 8;
	this->ctx.data[61] = this->ctx.bitlen >> 16;
	this->ctx.data[60] = this->ctx.bitlen >> 24;
	this->ctx.data[59] = this->ctx.bitlen >> 32;
	this->ctx.data[58] = this->ctx.bitlen >> 40;
	this->ctx.data[57] = this->ctx.bitlen >> 48;
	this->ctx.data[56] = this->ctx.bitlen >> 56;
	this->transform();

	// Since this implementation uses little endian byte ordering and MD uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i] =
			(this->ctx.state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] =
			(this->ctx.state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] =
			(this->ctx.state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] =
			(this->ctx.state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] =
			(this->ctx.state[4] >> (24 - i * 8)) & 0x000000ff;
	}

	return hash;
}

uint8_t *Sha1::serialize(uint32_t *lenptr) {
	return this->ctx.serialize(lenptr);
}
