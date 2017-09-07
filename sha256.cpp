#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include "sha256.hpp"

Sha256::Sha256(uint8_t data[SHA256_STATE_SIZE]) {
    if (data == nullptr) {
        SHA256_Init(&this->ctx);
        return;
    }

    uint32_t *words = reinterpret_cast<uint32_t *>(data);

    off_t i = 0;

    for (i = 0; i < 8; i++) {
        this->ctx.h[i] = ntohl(words[i]);
    }

    this->ctx.Nl = ntohl(words[8]);
    this->ctx.Nh = ntohl(words[9]);

    for (i = 0; i < 16; i++) {
        this->ctx.data[i] = ntohl(words[i + 10]);
    }

    this->ctx.num = ntohl(words[26]);
    this->ctx.md_len = ntohl(words[27]);
}

void Sha256::update(uint8_t *data, size_t len) {
    SHA256_Update(&this->ctx, data, len);
}

uint8_t *Sha256::finalize(size_t *lenptr) {
    *lenptr = SHA256_DIGEST_LENGTH;
    uint8_t *hash = (uint8_t *)malloc(SHA256_DIGEST_LENGTH);
    SHA256_Final(hash, &this->ctx);
    return hash;
}

uint8_t *Sha256::serialize(size_t *lenptr) {
    uint32_t *data = (uint32_t*)malloc(SHA256_STATE_SIZE);
    *lenptr = SHA256_STATE_SIZE;

    off_t i = 0;

    for (i = 0; i < 8; i++) {
        data[i] = htonl(this->ctx.h[i]);
    }

    data[8] = htonl(this->ctx.Nl);
    data[9] = htonl(this->ctx.Nh);

    for (i = 0; i < 16; i++) {
        data[i + 10] = htonl(this->ctx.data[i]);
    }

    data[26] = htonl(this->ctx.num);
    data[27] = htonl(this->ctx.md_len);

    return reinterpret_cast<uint8_t *>(data);
}
