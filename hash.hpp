#ifndef HASH_HPP
#define HASH_HPP

#include <stddef.h>
#include <stdint.h>

class Hash {
public:
  virtual void update(uint8_t data[], size_t len) = 0;
  virtual uint8_t *finalize(size_t *lenptr) = 0;

  virtual uint8_t *serialize(size_t *lenptr) = 0;
  virtual ~Hash() {}
};

#endif
