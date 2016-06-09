#ifndef SHA1_HPP
#define SHA1_HPP

#include <stddef.h>
#include "hash.hpp"

#define SHA1_BLOCK_SIZE 20
#define SHA1_STATE_SIZE 96

class Sha1: public Hash {
public:
	Sha1(uint8_t data[]);

	void update(uint8_t data[], size_t len);
  uint8_t *finalize(uint32_t *lenptr);

  uint8_t *serialize(uint32_t *lenptr);

private:
	struct {
		uint8_t data[64];
		uint32_t datalen;
		uint64_t bitlen;
		uint32_t state[5];
	} ctx;
	void transform();
};

#endif
