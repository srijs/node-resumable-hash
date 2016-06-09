#ifndef SHA256_HPP
#define SHA256_HPP

#include <stddef.h>
#include "hash.hpp"

#define SHA256_BLOCK_SIZE 32
#define SHA256_STATE_SIZE 108

class Sha256: public Hash {
public:
	Sha256(uint8_t data[]);

	void update(uint8_t data[], size_t len);
	uint8_t *finalize(uint32_t *lenptr);

  uint8_t *serialize(uint32_t *lenptr);

private:
	struct {
		uint8_t data[64];
		uint32_t datalen;
		uint64_t bitlen;
		uint32_t state[8];
	} ctx;
	void transform();
};

#endif
