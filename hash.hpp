#ifndef HASH_HPP
#define HASH_HPP

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <mutex>
#include <vector>

class Hash {
public:
    virtual ~Hash() {}

    void update(uint8_t data[], size_t len);
    std::vector<uint8_t> finalize();

    std::vector<uint8_t> serialize();

protected:
    virtual void update_(uint8_t data[], size_t len) = 0;
    virtual std::vector<uint8_t> finalize_() = 0;

    virtual std::vector<uint8_t> serialize_() = 0;

private:
    std::mutex mutex;
};

#endif
