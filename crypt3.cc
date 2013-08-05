/* Node.js Crypt(3) implementation */

#include <node.h>
#include <v8.h>

#include <unistd.h>		// for crypt if _XOPEN_SOURCE exists

using namespace v8;

Handle<Value> Method(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 2) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}

	if (!args[0]->IsString() || !args[1]->IsString()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}

	v8::String::Utf8Value key(args[0]->ToString());
	v8::String::Utf8Value salt(args[1]->ToString());

	Local<String> res = String::New( crypt(*key, *salt ) );
	return scope.Close(res);
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("crypt"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(crypt3, init)

/* EOF */
