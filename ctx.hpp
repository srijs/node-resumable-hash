#ifndef STATE_HPP
#define STATE_HPP

#include <stddef.h>
#include <stdlib.h>
#include <memory.h>

#define CTX_STATE_SIZE(N) (76 + N * 4)

template <unsigned int N>
class Ctx {
public:
	void init(uint32_t state[]);

  uint8_t *serialize(uint32_t *lenptr);
	void deserialize(uint8_t data[]);

	uint8_t data[64];
	uint32_t datalen;
	uint64_t bitlen;
	uint32_t state[N];
};

template <unsigned int N>
void Ctx<N>::init(uint32_t state[]) {
	this->datalen = 0;
	this->bitlen = 0;
	unsigned int i;
	for (i = 0; i < N; i++) {
		this->state[i] = state[i];
	}
}

template <unsigned int N>
void Ctx<N>::deserialize(uint8_t data[]) {
	unsigned int i;

	for (i = 0; i < 64; i++) {
		this->data[i] = data[i];
	}

	this->datalen = *(uint32_t *)(&data[64]);
	this->bitlen = *(uint64_t *)(&data[68]);

	uint32_t *data32 = (uint32_t *)(&data[76]);

	for (i = 0; i < N; i++) {
		this->state[i] = data32[i];
	}
}

template <unsigned int N>
uint8_t *Ctx<N>::serialize(uint32_t *lenptr) {
	*lenptr = CTX_STATE_SIZE(N);
	uint8_t *state = (uint8_t *)malloc(CTX_STATE_SIZE(N));
	unsigned int i;

	for (i = 0; i < 64; i++) {
		state[i] = this->data[i];
	}

	*(uint32_t *)(&state[64]) = this->datalen;
	*(uint64_t *)(&state[68]) = this->bitlen;

	uint32_t *state32 = (uint32_t *)(&state[76]);

	for (i = 0; i < N; i++) {
		state32[i] = this->state[i];
	}

	return state;
}

#endif
