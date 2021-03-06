#ifndef SHA1_HPP
#define SHA1_HPP

#include <stddef.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "hash.hpp"

#define SHA1_STATE_SIZE 96

class Sha1: public Hash {
public:
    Sha1(uint8_t data[SHA1_STATE_SIZE]);

protected:
    void update_(uint8_t *data, size_t len);
    std::vector<uint8_t> finalize_();

    std::vector<uint8_t> serialize_();

private:
    SHA_CTX ctx;
};

#endif
