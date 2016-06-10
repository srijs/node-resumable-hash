#ifndef SHA1_HPP
#define SHA1_HPP

#include <stddef.h>
#include <stdint.h>
#include "ctx.hpp"
#include "hash.hpp"

#define SHA1_BLOCK_SIZE 20
#define SHA1_STATE_SIZE CTX_STATE_SIZE(5)

class Sha1: public Hash {
public:
	Sha1(uint8_t data[]);

	void update(uint8_t data[], size_t len);
  uint8_t *finalize(uint32_t *lenptr);

  uint8_t *serialize(uint32_t *lenptr);

private:
	Ctx<5> ctx;
	void transform();
};

#endif
