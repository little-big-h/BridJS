#pragma once
#include <v8.h>
#include <node_buffer.h>
#include <sstream>



// return ThrowException(String::New("..."))
#define GET_INT32_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  if (!name ## Value->IsNumber()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for number argument " #name " at index " #index))); \
  } \
  Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
  int32_t name = (*name ## Number)->Int32Value();


#define GET_NUMBER_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  if (!name ## Value->IsNumber()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for number argument " #name " at index " #index))); \
  } \
  Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
  double name = (*name ## Number)->Value();
  
#define GET_STRING_ARG(name, args, index) \
  Local<Value> name = args[index]; \
  if (!name->IsString()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for string argument " #name " at index " #index))); \
  }
/*  
#define GET_POINTER_ARG(type, name, args, index) \
  GET_STRING_ARG(name ## Pointer, args, index); \
  type *name = (type*)bridjs::Utils::string2ptr(name ## Pointer);
*/  
#define GET_ASCII_STRING_ARG(name, args, index) \
  GET_STRING_ARG(name ## Value, args, index); \
  String::AsciiValue name ## AsciiValue(name ## Value); \
  const char *name = *name ## AsciiValue;

#define GET_POINTER_ARG(type, name, args, index) \
  Local<Value> name ## Value = args[index]; \
  if (!name ## Value->IsObject()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for number argument " #name " at index " #index))); \
  } \
  Local<Object> name ## Number = Local<Object>::Cast(name ## Value); \
  type* name = (type*)bridjs::Utils::unwrapPointer(name ## Number);

#define THROW_EXCEPTION(message) ThrowException(Exception::Error(String::New(message)))

#define CHECK_ARGUMENT(args, num) \
	if(args.Length()<num){ \
		std::stringstream message; \
		message<<"Illegal argument, it only had"<<args.Length()<<" arguments, but the number of arguments should be "<<num; \
		return THROW_EXCEPTION(message.str().c_str()); \
	} 

namespace bridjs {

	class Utils{
	public:
		/*
		static void* string2ptr(v8::Local<v8::Value> value);
		static v8::Handle<v8::Value> ptr2string(void* ptr);
		*/

		static v8::Handle<v8::Value> wrapPointer(const void* ptr);
		static void* unwrapPointer(v8::Local<v8::Object> buffer);
	};



}
