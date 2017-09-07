#include "hash.hpp"

void Hash::update(uint8_t data[], size_t len) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->update_(data, len);
}

uint8_t *Hash::finalize(size_t *lenptr) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->finalize_(lenptr);
}

uint8_t *Hash::serialize(size_t *lenptr) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->serialize_(lenptr);
}
