#include "nan.h"

#include "buffer.hpp"

v8::MaybeLocal<v8::Object> copy_to_buffer(std::vector<uint8_t> vec) {
    return Nan::CopyBuffer((char *)vec.data(), vec.size());
}