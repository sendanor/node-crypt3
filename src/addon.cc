#include <nan.h>
#include "crypt3.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("crypt3").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(crypt3)).ToLocalChecked());
}

NODE_MODULE(crypt3, InitAll)
