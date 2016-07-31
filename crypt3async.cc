/*
 *   An example of an asynchronous C++ node addon.
 *   Provided by paulhauner https://github.com/paulhauner
 *   License: MIT
 *   Tested in node.js v4.4.2 LTS in Ubuntu Linux
 */

#include <node.h>
#include <uv.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

namespace asyncAddon {
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;
	using v8::Persistent;
	using v8::Exception;

	/**
	 * Work structure is be used to pass the callback function and data 
	 * from the initiating function to the function which triggers the callback.
	 */
	struct Work {
		uv_work_t request;
		Persistent<Function> callback;
		int error;
		string key;
		string salt;
		string result;
	};

	/**
	 * WorkAsync function is the "middle" function which does the work.
	 * After the WorkAsync function is called, the WorkAsyncComplete function
	 * is called.
	 */
	static void WorkAsync(uv_work_t *req) {
		Work *work = static_cast<Work *>(req->data);

		const char *key = work->key.c_str();
		const char *salt = work->salt.c_str();

		char* res = crypt(key, salt);
		char* error;

		if (res != NULL) {
			work->error = 0;
			work->result.assign(res);
		} else {
			work->error = errno;
			error = strerror(errno);
			work->result.assign(error);
		}
	}

	/**
	 * WorkAsyncComplete function is called once we are ready to trigger the callback
	 * function in JS.
	 */
	static void WorkAsyncComplete(uv_work_t *req,int status) {
		Isolate * isolate = Isolate::GetCurrent();
		v8::HandleScope handleScope(isolate);
		Work *work = static_cast<Work *>(req->data);

		const int error = work->error;
		const char *result = work->result.c_str();
		if(error == 0) {
			Local<Value> argv[2] = {
				Undefined(isolate),
				String::NewFromUtf8(isolate, result)
			};
			// https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065
			Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);
		} else {
			Local<Value> argv[1] = { String::NewFromUtf8(isolate, result) };
			// https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065
			Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 1, argv);
		}

		work->callback.Reset();
		delete work;
	}

	/**
	 * Crypt3Async is the initial function called from JS. This function returns
	 * immediately, however starts a uv task which later calls the callback function
	 */
	void Crypt3Async(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Work * work = new Work();
		work->request.data = work;

		if (args.Length() < 3) {
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsFunction() ) {
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}

		v8::String::Utf8Value v8key(args[0]->ToString());
		string key = std::string(*v8key);
		work->key = key;

		v8::String::Utf8Value v8salt(args[1]->ToString());
		string salt = std::string(*v8salt);
		work->salt = salt;

		// args[2] is where we pick the callback function out of the JS function params.
		// Because we chose args[2], we must supply the callback fn as the first parameter
		Local<Function> callback = Local<Function>::Cast(args[2]);
		work->callback.Reset(isolate, callback);

		uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);

		args.GetReturnValue().Set(Undefined(isolate));
	}

	/**
	 * init function declares what we will make visible to node
	 */
	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "crypt", Crypt3Async);
	}

	NODE_MODULE(asyncAddon, init)
}
