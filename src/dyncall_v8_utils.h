#pragma once
#include <node_buffer.h>
#include <sstream>

#define EXPORT_FUNCTION(obj, ns, name) NODE_SET_METHOD(obj, #name, ns::name);

#define GET_CALL_ARG(args) \
	GET_POINTER_ARG(DCCallVM,pVm,args,0)\
	GET_POINTER_ARG(char,pFunction,args,1);

#define GET_INT32_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
  int32_t name;\
  if (name ## Value->IsNumber()) { \
   Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
    name = (*name ## Number)->Int32Value();\
  }else if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
	name = atol(*str); \
  }else{ \
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for int32_t argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
  }\

#define GET_FLOAT_ARG(name, args, index) \
  Local<Value> name ## Value = args[index]; \
 float name; \
 if (name ## Value->IsNumber()) { \
    Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
	name = static_cast<float>(name ## Number->Value()); \
  }else if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
	name = static_cast<float>(atof(*str)); \
  }else{ \
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for float argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
  } 

#define GET_INT64_ARG(name, args, index) \
 Local<Value> name ## Value = args[index]; \
 int64_t name; \
 if (name ## Value->IsNumber()) { \
	name = name ## Value->IntegerValue(); \
  }else if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
	name = atol(*str); \
  }else{ \
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for int64_t argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
  } 

#define GET_DOUBLE_ARG(name, args, index) \
 Local<Value> name ## Value = args[index]; \
 double name; \
 if (name ## Value->IsNumber()) { \
    Local<Number> name ## Number = Local<Number>::Cast(name ## Value); \
	name = name ## Number->Value(); \
  }else if(name ## Value->IsString()){ \
    Local<String> stringValue = Local<String>::Cast(name ## Value); \
	v8::String::Utf8Value str(name ## Value); \
	name = atof(*str); \
  }else{ \
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for double argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
  } 

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
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for bool argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
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
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for char argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
  } 
  
#define GET_STRING_ARG(name, args, index) \
  Local<Value> name = args[index]; \
  if (!name->IsString()) { \
    std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for string argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
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
  type* name; \
  if(name ## Value->IsString()){ \
	  v8::String::Utf8Value str(name ## Value); \
	  name = (type*)(*name ## Value); \
  }else if (name ## Value->IsObject() || name ## Value->IsNull()) { \
    name = (type*)bridjs::Utils::unwrapPointer(name ## Value); \
  }else{ \
	std::stringstream message; \
	v8::String::Utf8Value valueStr(args[index]); \
	message<< "Invalid value for pointer argument " #name " at index ="<<index<<", value = "<<(*valueStr); \
    return v8::ThrowException(v8::String::New(message.str().c_str())); \
  } 
  

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
		static v8::Handle<v8::Value> wrapPointerToBuffer(const void* ptr);
		static void* unwrapBufferToPointer(v8::Local<v8::Value> value);
		static v8::Handle<v8::Value> wrapPointer(const void* ptr);
		static const void* unwrapPointer(v8::Local<v8::Value> buffer);
		static v8::Local<v8::String> toV8String(const char val);
	};



}
