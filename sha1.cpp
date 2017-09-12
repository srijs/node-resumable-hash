#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include "sha1.hpp"

Sha1::Sha1(uint8_t data[SHA1_STATE_SIZE]) {
    if (data == nullptr) {
        SHA1_Init(&this->ctx);
        return;
    }

    auto words = reinterpret_cast<uint32_t *>(data);

    this->ctx.h0 = ntohl(words[0]);
    this->ctx.h1 = ntohl(words[1]);
    this->ctx.h2 = ntohl(words[2]);
    this->ctx.h3 = ntohl(words[3]);
    this->ctx.h4 = ntohl(words[4]);

    this->ctx.Nl = ntohl(words[5]);
    this->ctx.Nh = ntohl(words[6]);

    off_t i = 0;
    for (i = 0; i < 16; i++) {
        this->ctx.data[i] = ntohl(words[i + 7]);
    }

    this->ctx.num = ntohl(words[23]);
}

void Sha1::update_(uint8_t *data, size_t len) {
    SHA1_Update(&this->ctx, data, len);
}

std::vector<uint8_t> Sha1::finalize_() {
    std::vector<uint8_t> buffer(SHA_DIGEST_LENGTH);
    SHA1_Final(buffer.data(), &this->ctx);
    return buffer;
}

std::vector<uint8_t> Sha1::serialize_() {
    std::vector<uint8_t> buffer(SHA1_STATE_SIZE);
    auto data = reinterpret_cast<uint32_t *>(buffer.data());

    data[0] = htonl(this->ctx.h0);
    data[1] = htonl(this->ctx.h1);
    data[2] = htonl(this->ctx.h2);
    data[3] = htonl(this->ctx.h3);
    data[4] = htonl(this->ctx.h4);

    data[5] = htonl(this->ctx.Nl);
    data[6] = htonl(this->ctx.Nh);

    off_t i = 0;
    for (i = 0; i < 16; i++) {
        data[i + 7] = htonl(this->ctx.data[i]);
    }

    data[23] = htonl(this->ctx.num);

    return buffer;
}
