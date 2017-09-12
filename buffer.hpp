#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "stdint.h"
#include "stddef.h"

#include "v8.h"

v8::MaybeLocal<v8::Object> copy_to_buffer(std::vector<uint8_t> vec);

#endif
