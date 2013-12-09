#pragma once
#include <node_buffer.h>
#include <sstream>

#define GET_CALL_ARG(args) \
	GET_POINTER_ARG(DCCallVM,pVm,args,0)\
	GET_POINTER_ARG(char,pFunction,args,1);

#define GET_INT32_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  if (!name ## Value->IsNumber()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for int32 argument " #name " at index " #index))); \
  } \
  Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
  int32_t name = (*name ## Number)->Int32Value();

#define GET_FLOAT_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  if (!name ## Value->IsNumber()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for float argument " #name " at index " #index))); \
  } \
  Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
  float name = (float)(*name ## Number)->Value();

#define GET_INT64_ARG(name, args, index) \
 Local<Value> name ## Value = args[index]; \
 int64_t name; \
 if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
	name = atol(*str); \
  }else if (name ## Value->IsNumber()) { \
	name = name ## Value->IntegerValue(); \
  }else{ \
	return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for int64_t argument " #name " at index " #index))); \
  } 

#define GET_DOUBLE_ARG(name, args, index) \
 Local<Value> name ## Value = args[index]; \
 double name; \
 if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
	name = atof(*str); \
  }else if (name ## Value->IsNumber()) { \
    Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
	name = name ## Number->Value(); \
  }else{ \
	return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for double argument " #name " at index " #index))); \
  } 

#define GET_NUMBER_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  if (!name ## Value->IsNumber()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for number argument " #name " at index " #index))); \
  } \
  Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
  double name = (*name ## Number)->Value();

#define GET_BOOL_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  bool name; \
  if (name ## Value->IsBoolean()) { \
	name = name ## Value->ToBoolean()->BooleanValue(); \
  }else if(name ## Value->IsBooleanObject()){ \
	name = Local<BooleanObject>::Cast(name ## Value)->BooleanValue(); \
  }else if (name ## Value->IsNumber()) { \
    name = Local<Number>::Cast(name ## Value)->BooleanValue(); \
  }else{ \
	return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for boolean argument " #name " at index " #index))); \
  } 

#define GET_CHAR_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  char name; \
  if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
    if(str.length()!=1){ \
		std::stringstream message; \
		message<<"Illegal cahr value: "<<(*str); \
		return v8::ThrowException(v8::Exception::Error(String::New(message.str().c_str()))); \
	} \
	name = (*str)[0]; \
  }else if (name ## Value->IsNumber()) { \
	name = Local<Number>::Cast(name ## Value)->Int32Value(); \
  }else{ \
	return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for char argument " #name " at index " #index))); \
  } 
  
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
  if (!name ## Value->IsObject() && !name ## Value->IsNull()) { \
    return v8::ThrowException(v8::Exception::TypeError(String::New( \
      "Invalid value for number argument " #name " at index " #index))); \
  } \
  type* name = (type*)bridjs::Utils::unwrapPointer(name ## Value);

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
		static v8::Handle<v8::Value> wrapPointer(const void* ptr);
		static void* unwrapPointer(v8::Local<v8::Value> buffer);
	};



}
