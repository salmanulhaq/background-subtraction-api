#include "bgsub.h"

#include <iostream>

using namespace std;
using namespace v8;
using namespace node;

//#define DEBUG

class SubtractWorker : public NanAsyncWorker {
 public:
  SubtractWorker(string url, NanCallback *callback)
    : NanAsyncWorker(callback), url(url), contentLength(0.0), status(NO_ERROR){}
  ~SubtractWorker() {}

  void Execute() {
    #ifdef DEBUG 
      std::cout<<"bgsub.cc: calling subtract()"<<std::endl; 
    #endif
    
    status = subtract(url, contentLength, output);
    #ifdef DEBUG 
      std::cout<<"bgsub.cc: subtract() called, status: "<<status<<std::endl; 
    #endif
  }

  //callback: error, extension, image
  void HandleOKCallback () {
    NanScope();

    #ifdef DEBUG 
      std::cout<<"bgsub.cc: inside HandleOKCallback()"<<std::endl; 
    #endif

    if(status == SUCCESS) {
      Local<Object> encodedImageBuffer = NanNewBufferHandle(output.size());
      uchar* bufferPtr = (uchar*)Buffer::Data(encodedImageBuffer);
      memcpy(bufferPtr, &output[0], output.size());
      Local<Value> argv[] = {
          NanNull(),
          NanNew(contentLength),
          encodedImageBuffer
      };
      #ifdef DEBUG 
        std::cout<<"bgsub.cc: status==SUCCESS, calling callback->Call()"<<std::endl; 
      #endif
      callback->Call(3, argv);
      #ifdef DEBUG 
        std::cout<<"bgsub.cc: callback->Call() successful"<<std::endl; 
      #endif
    } else {
      #ifdef DEBUG 
        std::cout<<"bgsub.cc: status != SUCCESS, sending error callback"<<std::endl; 
      #endif
      Local<Value> argv[] = {
          NanNew(string("ERROR")),
          NanNull(),
          NanNull()
      };

      callback->Call(3, argv);
      #ifdef DEBUG 
        std::cout<<"bgsub.cc: error callback sent"<<std::endl; 
      #endif
    }
  }

 private:
  string url;
  double contentLength;
  vector<uchar> output;
  BGSubError status;
};

NAN_METHOD(Subtract) {
  NanScope();
  #ifdef DEBUG 
      std::cout<<"bgsub.cc: in Subtract(), parsing"<<std::endl; 
    #endif
  string url = string(*(NanAsciiString(args[0])));

  NanCallback *callback = new NanCallback(args[1].As<Function>());

  NanAsyncQueueWorker(new SubtractWorker(url, callback));
  #ifdef DEBUG 
    std::cout<<"bgsub.cc: launched new async worker"<<std::endl; 
  #endif
  NanReturnUndefined();
}



