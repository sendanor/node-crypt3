/* Node.js Crypt(3) implementation */
#include "crypt3.h"

#include <errno.h>
#include <unistd.h> // for crypt if _XOPEN_SOURCE exists

using Nan::New;
using Nan::ThrowError;
using Nan::ThrowTypeError;
using Nan::NanErrnoException;

NAN_METHOD(crypt3) {
  if (info.Length() != 2) {
    return Nan::ThrowTypeError("Wrong number of arguments");
  }

  if (!info[0]->IsString() || !info[1]->IsString()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  v8::String::Utf8Value key(info[0]->ToString());
  v8::String::Utf8Value salt(info[1]->ToString());

  char* res = crypt(*key, *salt);

  if (res != NULL) {
    info.GetReturnValue().Set(Nan::New(res).ToLocalChecked());
  }

  else {
    return Nan::ThrowError(Nan::NanErrnoException(errno, "crypt"));
  }
}
