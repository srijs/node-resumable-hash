#include "hash.hpp"

void Hash::update(uint8_t data[], size_t len) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->update_(data, len);
}

std::vector<uint8_t> Hash::finalize() {
    std::lock_guard<std::mutex> lock(this->mutex);
    auto buffer = this->finalize_();
    return buffer;
}

std::vector<uint8_t> Hash::serialize() {
    std::lock_guard<std::mutex> lock(this->mutex);
    auto buffer = this->serialize_();
    return buffer;
}
