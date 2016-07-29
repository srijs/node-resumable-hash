#include <node.h>
#include <nan.h>

#include "hash.hpp"
#include "sha1.hpp"
#include "sha256.hpp"

using v8::Exception;
using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::Error;
using Nan::HandleScope;
using Nan::MakeCallback;
using Nan::Null;
using Nan::ObjectWrap;
using Nan::Persistent;
using Nan::Undefined;
using Nan::Utf8String;

static Persistent<FunctionTemplate> constructor;

class UpdateWorker: public AsyncWorker {
  Hash *hash;
  uint8_t *data;
  size_t len;

public:
  UpdateWorker(Callback *callback, Hash *hash, Local<Object> &buf)
    : AsyncWorker(callback), hash(hash) {
      data = (uint8_t *)node::Buffer::Data(buf);
      len = node::Buffer::Length(buf);
      SaveToPersistent(0u, buf);
    }

  void Execute() {
    this->hash->update(data, len);
  }

  void HandleOKCallback() {
    HandleScope();
    callback->Call(0, nullptr);
  }
};

class FinalizeWorker: public AsyncWorker {
  Hash *hash;
  uint8_t *data;
  uint32_t len;

public:
  FinalizeWorker(Callback *callback, Hash *hash)
    : AsyncWorker(callback), hash(hash) {}

  void Execute() {
    data = this->hash->finalize(&len);
  }

  void HandleOKCallback() {
    HandleScope();
    Local<Value> argv[1];
    argv[0] = Nan::NewBuffer((char *)data, len).ToLocalChecked();
    callback->Call(1, argv);
  }
};

class HashEngine: public ObjectWrap {
  Hash *hash;

  explicit HashEngine(Hash *hash) {
    this->hash = hash;
  }

public:
  virtual ~HashEngine() {
    delete hash;
  }

  static void Init() {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(HashEngine::New);
    constructor.Reset(tpl);
    tpl->SetClassName(Nan::New<String>("HashEngine").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    SetPrototypeMethod(tpl, "update", HashEngine::UpdateAsync);
    SetPrototypeMethod(tpl, "finalizeSync", HashEngine::Finalize);
    SetPrototypeMethod(tpl, "finalize", HashEngine::FinalizeAsync);
    SetPrototypeMethod(tpl, "serialize", HashEngine::Serialize);
  }

  static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
    HandleScope();
    HashEngine* self;
    if (info.Length() < 1) {
      return Nan::ThrowError("You must provide one argument.");
    }
    Local<Number> typ = info[0].As<Number>();
    if (typ->Value() == 0) {
      uint8_t *data = nullptr;
      if (info.Length() > 1) {
        Local<Object> buf = info[1].As<Object>();
        if (!node::Buffer::HasInstance(buf)) {
          return Nan::ThrowError("Argument should be a buffer object.");
        }
        if (node::Buffer::Length(buf) < SHA1_STATE_SIZE) {
          return Nan::ThrowError("Buffer is too small.");
        }
        data = (uint8_t *)node::Buffer::Data(buf);
      }
      Sha1 *sha1 = new Sha1(data);
      self = new HashEngine(sha1);
    } else if (typ->Value() == 1) {
      uint8_t *data = nullptr;
      if (info.Length() > 1) {
        Local<Object> buf = info[1].As<Object>();
        if (!node::Buffer::HasInstance(buf)) {
          return Nan::ThrowError("Argument should be a buffer object.");
        }
        if (node::Buffer::Length(buf) < SHA256_STATE_SIZE) {
          return Nan::ThrowError("Buffer is too small.");
        }
        data = (uint8_t *)node::Buffer::Data(buf);
      }
      Sha256 *sha256 = new Sha256(data);
      self = new HashEngine(sha256);
    } else {
      return Nan::ThrowError("Unknown hash type.");
    }
    self->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  static void Serialize(Nan::NAN_METHOD_ARGS_TYPE info) {
    HandleScope();
    HashEngine* self = ObjectWrap::Unwrap<HashEngine>(info.This());
    uint32_t len;
    uint8_t *data = self->hash->serialize(&len);
    info.GetReturnValue().Set(Nan::NewBuffer((char *)data, len).ToLocalChecked());
  }

  static void UpdateAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
    HandleScope();
    HashEngine* self = ObjectWrap::Unwrap<HashEngine>(info.This());
    if (info.Length() < 2) {
      return Nan::ThrowError("You must provide two arguments.");
    }
    Local<Object> buf = info[0].As<Object>();
    if (!node::Buffer::HasInstance(buf)) {
      return Nan::ThrowError("Argument should be a buffer object.");
    }
    Callback *callback = new Callback(info[1].As<Function>());
    AsyncQueueWorker(new UpdateWorker(callback, self->hash, buf));
  }

  static void Finalize(Nan::NAN_METHOD_ARGS_TYPE info) {
    HandleScope();
    HashEngine* self = ObjectWrap::Unwrap<HashEngine>(info.This());
    uint32_t len;
    uint8_t *data = self->hash->finalize(&len);
    info.GetReturnValue().Set(Nan::NewBuffer((char *)data, len).ToLocalChecked());
  }

  static void FinalizeAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
    HandleScope();
    HashEngine* self = ObjectWrap::Unwrap<HashEngine>(info.This());
    if (info.Length() < 1) {
      return Nan::ThrowError("You must provide one argument.");
    }
    Callback *callback = new Callback(info[0].As<Function>());
    AsyncQueueWorker(new FinalizeWorker(callback, self->hash));
  }
};

void Init(Handle<Object> exports, Handle<Object> module) {
  HashEngine::Init();
  Local<FunctionTemplate> constructorHandle = New(constructor);
  module->Set(Nan::New<String>("exports").ToLocalChecked(), constructorHandle->GetFunction());
}

NODE_MODULE(engine, Init)
