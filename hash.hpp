#ifndef HASH_HPP
#define HASH_HPP

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <mutex>

class Hash {
public:
    virtual void update(uint8_t data[], size_t len) = 0;
    virtual uint8_t *finalize(size_t *lenptr) = 0;

    virtual uint8_t *serialize(size_t *lenptr) = 0;
    virtual ~Hash() {}
};

class ThreadSafeHash {
public:
    ThreadSafeHash(Hash *inner);
    ~ThreadSafeHash();

    void update(uint8_t data[], size_t len);
    uint8_t *finalize(size_t *lenptr);

    uint8_t *serialize(size_t *lenptr);

private:
    std::unique_ptr<Hash> inner;
    std::mutex mutex;
};

#endif
