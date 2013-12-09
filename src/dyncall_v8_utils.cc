#include "dyncall_v8_utils.h"

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
Handle<Value> bridjs::Utils::wrapPointer(const void* ptr){
	HandleScope scope;
	Handle<Value> result;
	void* pptr;

	if(ptr!=NULL){
		node::Buffer *buf = node::Buffer::New((char*)(&ptr), sizeof(void*));
		result =  scope.Close(buf->handle_);
	}else{
		result = scope.Close(v8::Null());
	}

	return result;

	//memcpy(&pptr,node::Buffer::Data(buf), sizeof(void*));

	
}


void* bridjs::Utils::unwrapPointer(v8::Local<v8::Value> value){
	void* ptr;

	memcpy(&ptr, node::Buffer::Data(value->ToObject()), sizeof(void*));

	return ptr;
}