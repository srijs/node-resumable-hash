#include <stdlib.h>
#include <memory.h>
#include "sha1.hpp"

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))

static const uint32_t k[4] = {
	0x5a827999,
	0x6ed9eba1,
	0x8f1bbcdc,
	0xca62c1d6
};

Sha1::Sha1(uint8_t data[]) {
	if (data == nullptr) {
		this->ctx.datalen = 0;
		this->ctx.bitlen = 0;
		this->ctx.state[0] = 0x67452301;
		this->ctx.state[1] = 0xEFCDAB89;
		this->ctx.state[2] = 0x98BADCFE;
		this->ctx.state[3] = 0x10325476;
		this->ctx.state[4] = 0xc3d2e1f0;
	} else {
		int i;

		for (i = 0; i < 64; i++) {
			this->ctx.data[i] = data[i];
		}

		this->ctx.datalen = *(uint32_t *)(&data[64]);
		this->ctx.bitlen = *(uint64_t *)(&data[68]);

		uint32_t *data32 = (uint32_t *)(&data[76]);

		for (i = 0; i < 5; i++) {
			this->ctx.state[i] = data32[i];
		}
	}
}

void Sha1::transform() {
	uint32_t a, b, c, d, e, i, j, t, m[80];

	for (i = 0, j = 0; i < 16; ++i, j += 4) {
		m[i] =
			(this->ctx.data[j] << 24) +
			(this->ctx.data[j + 1] << 16) +
			(this->ctx.data[j + 2] << 8) +
			(this->ctx.data[j + 3]);
	}
	for ( ; i < 80; ++i) {
		m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
		m[i] = (m[i] << 1) | (m[i] >> 31);
	}

	a = this->ctx.state[0];
	b = this->ctx.state[1];
	c = this->ctx.state[2];
	d = this->ctx.state[3];
	e = this->ctx.state[4];

	for (i = 0; i < 20; ++i) {
		t = ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + k[0] + m[i];
		e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
	}
	for ( ; i < 40; ++i) {
		t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + k[1] + m[i];
		e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
	}
	for ( ; i < 60; ++i) {
		t = ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d))  + e + k[2] + m[i];
		e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
	}
	for ( ; i < 80; ++i) {
		t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + k[3] + m[i];
		e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
	}

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
	*lenptr = SHA1_STATE_SIZE;
	uint8_t *state = (uint8_t *)malloc(SHA1_STATE_SIZE);
	int i;

	for (i = 0; i < 64; i++) {
		state[i] = this->ctx.data[i];
	}

	*(uint32_t *)(&state[64]) = this->ctx.datalen;
	*(uint64_t *)(&state[68]) = this->ctx.bitlen;

	uint32_t *state32 = (uint32_t *)(&state[76]);

	for (i = 0; i < 5; i++) {
		state32[i] = this->ctx.state[i];
	}

	return state;
}
