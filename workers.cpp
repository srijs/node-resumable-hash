#include "workers.hpp"

using v8::Local;
using v8::Value;

using Nan::HandleScope;

UpdateWorker::UpdateWorker(Callback *callback, std::shared_ptr<Hash> hash, Local<Object> &buf)
    : AsyncWorker(callback), hash(hash)
{
    data = (uint8_t *)node::Buffer::Data(buf);
    len = node::Buffer::Length(buf);
    SaveToPersistent(0u, buf);
}

void UpdateWorker::Execute()
{
    this->hash->update(data, len);
}

void UpdateWorker::HandleOKCallback()
{
    HandleScope();
    callback->Call(0, nullptr);
}

FinalizeWorker::FinalizeWorker(Callback *callback, std::shared_ptr<Hash> hash)
    : AsyncWorker(callback), hash(hash) {}

void FinalizeWorker::Execute()
{
    data = this->hash->finalize();
}

void FinalizeWorker::HandleOKCallback()
{
    HandleScope();
    Local<Value> argv[1];
    argv[0] = copy_to_buffer(data).ToLocalChecked();
    callback->Call(1, argv);
}
