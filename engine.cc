#include <node.h>
#include <nan.h>

#include "hash.hpp"
#include "sha1.hpp"
#include "sha256.hpp"
#include "workers.hpp"
#include "buffer.hpp"

using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

using Nan::AsyncQueueWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::ObjectWrap;
using Nan::Persistent;
using Nan::Utf8String;

static Persistent<FunctionTemplate> constructor;

enum HashType
{
    HashTypeSha1,
    HashTypeSha256
};

class HashEngine : public ObjectWrap
{
    std::shared_ptr<Hash> hash;

    explicit HashEngine(HashType hash_type, uint8_t *init_payload)
    {
        switch (hash_type)
        {
        case HashTypeSha1:
            hash = std::make_shared<Sha1>(init_payload);
            break;
        case HashTypeSha256:
            hash = std::make_shared<Sha256>(init_payload);
            break;
        }
    }

    static size_t ExpectedInitPayloadSize(HashType hash_type)
    {
        switch (hash_type)
        {
        case HashTypeSha1:
            return SHA1_STATE_SIZE;
        case HashTypeSha256:
            return SHA256_STATE_SIZE;
        }
    }

    static uint8_t *ValidateInitPayloadBuffer(Local<Object> buf, HashType hash_type)
    {
        if (!node::Buffer::HasInstance(buf))
        {
            return nullptr;
        }
        if (node::Buffer::Length(buf) != ExpectedInitPayloadSize(hash_type))
        {
            return nullptr;
        }
        return (uint8_t *)node::Buffer::Data(buf);
    }

public:
    static void Init()
    {
        auto tpl = Nan::New<FunctionTemplate>(HashEngine::New);
        constructor.Reset(tpl);
        tpl->SetClassName(Nan::New<String>("HashEngine").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        SetPrototypeMethod(tpl, "updateSync", HashEngine::Update);
        SetPrototypeMethod(tpl, "update", HashEngine::UpdateAsync);
        SetPrototypeMethod(tpl, "finalizeSync", HashEngine::Finalize);
        SetPrototypeMethod(tpl, "finalize", HashEngine::FinalizeAsync);
        SetPrototypeMethod(tpl, "serialize", HashEngine::Serialize);
    }

    static void New(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        HandleScope();
        if (info.Length() < 1)
        {
            return Nan::ThrowError("You must provide one argument.");
        }

        enum HashType hash_type;
        switch ((int)info[0].As<Number>()->Value())
        {
        case 0:
            hash_type = HashTypeSha1;
            break;
        case 1:
            hash_type = HashTypeSha256;
            break;
        default:
            return Nan::ThrowError("Unknown hash type.");
        }

        uint8_t *init_payload = nullptr;
        if (info.Length() > 1)
        {
            init_payload = ValidateInitPayloadBuffer(info[1].As<Object>(), hash_type);
            if (init_payload == nullptr)
            {
                return Nan::ThrowError("Invalid init payload.");
            }
        }

        auto self = new HashEngine(hash_type, init_payload);
        self->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }

    static void Serialize(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        HandleScope();
        auto self = ObjectWrap::Unwrap<HashEngine>(info.This());
        auto data = self->hash->serialize();
        info.GetReturnValue().Set(copy_to_buffer(data).ToLocalChecked());
    }

    static void Update(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        HandleScope();
        auto self = ObjectWrap::Unwrap<HashEngine>(info.This());
        if (info.Length() < 1)
        {
            return Nan::ThrowError("You must provide one argument.");
        }
        auto buf = info[0].As<Object>();
        if (!node::Buffer::HasInstance(buf))
        {
            return Nan::ThrowError("Argument should be a buffer object.");
        }
        auto data = (uint8_t *)node::Buffer::Data(buf);
        auto len = node::Buffer::Length(buf);
        self->hash->update(data, len);
    }

    static void UpdateAsync(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        HandleScope();
        auto self = ObjectWrap::Unwrap<HashEngine>(info.This());
        if (info.Length() < 2)
        {
            return Nan::ThrowError("You must provide two arguments.");
        }
        auto buf = info[0].As<Object>();
        if (!node::Buffer::HasInstance(buf))
        {
            return Nan::ThrowError("Argument should be a buffer object.");
        }
        auto callback = new Callback(info[1].As<Function>());
        AsyncQueueWorker(new UpdateWorker(callback, self->hash, buf));
    }

    static void Finalize(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        HandleScope();
        auto self = ObjectWrap::Unwrap<HashEngine>(info.This());
        auto data = self->hash->finalize();
        info.GetReturnValue().Set(copy_to_buffer(data).ToLocalChecked());
    }

    static void FinalizeAsync(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        HandleScope();
        auto self = ObjectWrap::Unwrap<HashEngine>(info.This());
        if (info.Length() < 1)
        {
            return Nan::ThrowError("You must provide one argument.");
        }
        auto callback = new Callback(info[0].As<Function>());
        AsyncQueueWorker(new FinalizeWorker(callback, self->hash));
    }
};

void Init(Local<Object> exports, Local<Object> module)
{
    HashEngine::Init();
    Local<FunctionTemplate> constructorHandle = New(constructor);
    module->Set(Nan::New<String>("exports").ToLocalChecked(), constructorHandle->GetFunction());
}

NODE_MODULE(engine, Init)
