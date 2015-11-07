/* Node.js Crypt(3) implementation */

#include <v8.h>
#include <errno.h>
#include <unistd.h>		// for crypt if _XOPEN_SOURCE exists
#include <nan.h>

using namespace v8;

NAN_METHOD(Method) {
	Nan::HandleScope scope;

	if (info.Length() < 2) {
		return Nan::ThrowTypeError("Wrong number of arguments");
	}

	if (!info[0]->IsString() || !info[1]->IsString()) {
		return Nan::ThrowTypeError("Wrong arguments");
	}

	v8::String::Utf8Value key(info[0]->ToString());
	v8::String::Utf8Value salt(info[1]->ToString());

	char* res = crypt(*key, *salt);
	if (res != NULL) {
		info.GetReturnValue().Set(Nan::New<String>(res).ToLocalChecked());
	} else {
		return Nan::ThrowError(Nan::ErrnoException(errno, "crypt"));
	}
}

void init(Handle<Object> exports) {
	exports->Set(Nan::New<String>("crypt").ToLocalChecked(),
		Nan::New<FunctionTemplate>(Method)->GetFunction());
}

NODE_MODULE(crypt3, init)

/* EOF */
