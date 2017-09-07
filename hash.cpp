#include "hash.hpp"

ThreadSafeHash::ThreadSafeHash(Hash *inner)
    : inner(inner) {}

ThreadSafeHash::~ThreadSafeHash() {}

void ThreadSafeHash::update(uint8_t data[], size_t len) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->inner->update(data, len);
}

uint8_t *ThreadSafeHash::finalize(size_t *lenptr) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->inner->finalize(lenptr);
}

uint8_t *ThreadSafeHash::serialize(size_t *lenptr) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->inner->serialize(lenptr);
}
