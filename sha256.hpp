#ifndef SHA256_HPP
#define SHA256_HPP

#include <stddef.h>
#include <stdint.h>
#include "ctx.hpp"
#include "hash.hpp"

#define SHA256_BLOCK_SIZE 32
#define SHA256_STATE_SIZE CTX_STATE_SIZE(8)

class Sha256: public Hash {
public:
	Sha256(uint8_t data[SHA256_STATE_SIZE]);

	void update(uint8_t data[], size_t len);
	uint8_t *finalize(uint32_t *lenptr);

  uint8_t *serialize(uint32_t *lenptr);

private:
	Ctx<8> ctx;
	void transform();
};

#endif
