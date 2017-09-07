#ifndef HASH_HPP
#define HASH_HPP

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <mutex>

class Hash {
public:
    virtual ~Hash() {}

    void update(uint8_t data[], size_t len);
    uint8_t *finalize(size_t *lenptr);

    uint8_t *serialize(size_t *lenptr);

protected:
    virtual void update_(uint8_t data[], size_t len) = 0;
    virtual uint8_t *finalize_(size_t *lenptr) = 0;

    virtual uint8_t *serialize_(size_t *lenptr) = 0;

private:
    std::mutex mutex;
};

#endif
