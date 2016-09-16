#include "bgsub.h"
#include <stdio.h>

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;

void Init(Handle<Object> exports) {
  	exports->Set(NanNew<String>("subtract"), NanNew<FunctionTemplate>(Subtract)->GetFunction());
  	
}

NODE_MODULE(bgsub, Init)

