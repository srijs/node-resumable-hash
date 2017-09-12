#ifndef WORKERS_HPP
#define WORKERS_HPP

#include <node.h>
#include <nan.h>

#include "hash.hpp"
#include "buffer.hpp"

using v8::Local;
using v8::Object;

using Nan::AsyncWorker;
using Nan::Callback;

class UpdateWorker : public AsyncWorker {
public:
    UpdateWorker(Callback *callback, std::shared_ptr<Hash> hash, Local<Object> &buf);

    void Execute();
    void HandleOKCallback();

private:
    std::shared_ptr<Hash> hash;
    uint8_t *data;
    size_t len;
};

class FinalizeWorker : public AsyncWorker {
public:
    FinalizeWorker(Callback *callback, std::shared_ptr<Hash> hash);

    void Execute();
    void HandleOKCallback();

private:
    std::shared_ptr<Hash> hash;
    std::vector<uint8_t> data;
};

#endif
