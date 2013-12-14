#include "dyncall_v8_utils.h"
#include "pointer_v8.h"
#include <iostream>

using namespace v8;
/*
void* bridjs::Utils::string2ptr(Local<Value> value) {
  String::AsciiValue str(value);
  void* ptr;
  switch (sizeof(void*)) {
  case 8:
    unsigned long long ll;
    sscanf(*str, "%llx", &ll);
    ptr = (void*)ll;
    break;
  case 4:
    unsigned long i;
    sscanf(*str, "%lx", &i);
    ptr = (void*)ll;
    break;
  default:
    ptr = 0;
  }
  return ptr;
}

Handle<Value> bridjs::Utils::ptr2string(void* ptr) {
  char str[20];
  switch (sizeof(void*)) {
  case 8:
    sprintf(str, "%#16llx", (unsigned long long)(size_t)ptr);
    break;
  case 4:
    sprintf(str, "%#8lx", (unsigned long)(size_t)ptr);
    break;
  default:
    *str = 0;
  }
  return String::New(str);
}
*/

Handle<Value> bridjs::Utils::wrapPointerToBuffer(const void* ptr){
	HandleScope scope;
	Handle<Value> result;

	if(ptr!=NULL){
		node::Buffer *buf = node::Buffer::New((char*)(&ptr), sizeof(void*));
		result =  scope.Close(buf->handle_);
	}else{
		result = scope.Close(v8::Null());
	}

	return result;

	//memcpy(&pptr,node::Buffer::Data(buf), sizeof(void*));

	
}


void* bridjs::Utils::unwrapBufferToPointer(v8::Local<v8::Value> value){
	void* ptr;

	memcpy(&ptr, node::Buffer::Data(value->ToObject()), sizeof(void*));

	return ptr;
}

Handle<v8::Value> bridjs::Utils::wrapPointer(const void* ptr){
	HandleScope scope;
	Handle<v8::Value> result;
	/*
	if(ptr!=NULL){
		bridjs::Pointer *buf = bridjs::Pointer::New(ptr);
		result =  scope.Close(buf->handle_);
	}else{
		result = scope.Close(v8::Null());
	}*/

	//bridjs::Pointer *buf = bridjs::Pointer::New(ptr);
	result =  scope.Close(bridjs::Pointer::NewInstance(ptr));

	return result;

	//memcpy(&pptr,node::Buffer::Data(buf), sizeof(void*));
}


const void* bridjs::Utils::unwrapPointer(v8::Local<v8::Value> value){
	const void* ptr;

	//memcpy(&ptr, node::Buffer::Data(value->ToObject()), sizeof(void*));
	if(value->IsNull()){
		ptr = NULL;
	}else{
		ptr = bridjs::Pointer::Data(value->ToObject());
	}

	return ptr;
}

v8::Local<v8::String> bridjs::Utils::toV8String(const char val){
	//HandleScope scope;
	char str[] = {val, '\0'};

	return v8::String::New(str);
}