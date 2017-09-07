#ifndef SHA256_HPP
#define SHA256_HPP

#include <stddef.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "hash.hpp"

#define SHA256_STATE_SIZE 112

class Sha256: public Hash {
public:
    Sha256(uint8_t data[SHA256_STATE_SIZE]);

    void update(uint8_t *data, size_t len);
    uint8_t *finalize(size_t *lenptr);

    uint8_t *serialize(size_t *lenptr);

private:
    SHA256_CTX ctx;
};

#endif
