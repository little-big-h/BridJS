#include "dyncall_v8_utils.h"
#include "pointer_v8.h"
#include <iostream>

extern "C"
{
    #include "dyncall.h"    
	#include "dyncall_signature.h"
}

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
v8::Handle<v8::Value> WriteInt64(const v8::Arguments& args);

size_t bridjs::Utils::getTypeSize(const char type){
	size_t size;

		switch(type){
		/*
		case DC_SIGCHAR_VOID:{
			size = sizeof(DCbool);
			}
			break;*/
		case DC_SIGCHAR_BOOL:{
			size = sizeof(DCbool);
			}
			break;
		case DC_SIGCHAR_UCHAR:{
			size = sizeof(DCuchar);
			}
			break;
		case DC_SIGCHAR_CHAR:{
			size = sizeof(DCchar);
			}
			break;
        case DC_SIGCHAR_SHORT:{
			size = sizeof(DCshort);
			}
			break;
		case DC_SIGCHAR_USHORT:{
			size = sizeof(DCushort);
			}
			break;
        case DC_SIGCHAR_INT:{
			size = sizeof(DCint);
			}
			break;
		case DC_SIGCHAR_UINT:{
			size = sizeof(DCuint);
			}
			break; 
        case DC_SIGCHAR_LONG:{
			size = sizeof(DClong);
			}
			break;
		case DC_SIGCHAR_ULONG:{
			size = sizeof(DCulong);
			}
			break;
		case DC_SIGCHAR_LONGLONG:{
			size = sizeof(DClonglong);
			}
			break;
		case DC_SIGCHAR_ULONGLONG:{
			size = sizeof(DCulonglong);
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			size = sizeof(DCfloat);
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			size = sizeof(DCdouble);
			}
			break;
		case DC_SIGCHAR_STRING:
        case DC_SIGCHAR_POINTER:{
			size = sizeof(DCpointer);
			}
			break;
			/*
		case DC_SIGCHAR_STRUCT:{
			std::cerr<<"Not implement"<<std::endl;
			GET_POINTER_VALUE(void, rValue,returnValue,0);
			value->p = (rValue);
		    }
			break;*/

		default:
			std::stringstream message;
			message<<"Unknown type: "<<type<<std::endl;
			//throw std::runtime_error(message.str());
			throw std::runtime_error(message.str().c_str());
		}
	
		return size;
}

v8::Handle<v8::Value> GetTypeSize(const v8::Arguments& args){
	try{
		HandleScope scope;
		size_t size;

		GET_CHAR_ARG(type, args, 0);

		size =  bridjs::Utils::getTypeSize(type);

		return scope.Close(WRAP_ULONGLONG(size));
	}catch(std::exception &e){
		return v8::Exception::TypeError(v8::String::New(e.what()));
	}

}

v8::Handle<v8::Value> WriteInt64(const v8::Arguments& args){
	HandleScope scope;

	GET_POINTER_ARG(const char, ptr,args,0);
	GET_INT64_ARG(offset,args,1);
	GET_INT64_ARG(value,args,2);

	ptr += offset;

	(*(int64_t*)ptr) = value;

	return scope.Close(v8::Undefined());
}

void bridjs::Utils::Init(v8::Handle<v8::Object> utilsObj){
	NODE_SET_METHOD(utilsObj,"getTypeSize",GetTypeSize);
	NODE_SET_METHOD(utilsObj,"writeInt64",WriteInt64);
	NODE_SET_METHOD(utilsObj,"pointerToString",PointerToString);
}

v8::Handle<v8::Value> bridjs::Utils::PointerToString(const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(const char,ptr,args,0);
	v8::Local<v8::String> str;

	if(args.Length()>=2){
		GET_INT32_ARG(length,args,1);
		str = v8::String::New(ptr,length);
	}else{
		str = v8::String::New(ptr);
	}


	return scope.Close(str);
}

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
	
	if(ptr!=NULL){
		result =  scope.Close(bridjs::Pointer::NewInstance(ptr));
	}else{
		result = scope.Close(v8::Null());
	}

	//bridjs::Pointer *buf = bridjs::Pointer::New(ptr);
	//result =  scope.Close(bridjs::Pointer::NewInstance(ptr));

	return result;

	//memcpy(&pptr,node::Buffer::Data(buf), sizeof(void*));
}


const void* bridjs::Utils::unwrapPointer(v8::Local<v8::Value> value){
	const void* ptr;

	//memcpy(&ptr, node::Buffer::Data(value->ToObject()), sizeof(void*));
	if(value->IsNull()){
		ptr = NULL;
	}else if(node::Buffer::HasInstance(value)){
		ptr = node::Buffer::Data(value);
	}else if(value->IsObject()){
		v8::Local<v8::Object> object = value->ToObject();
		
		if(object->InternalFieldCount()>0){
			ptr = bridjs::Pointer::Data(object);
		}else{
			throw std::runtime_error("Invalid bridjs::Pointer object");
		}
	}else{
		throw std::runtime_error("Unknown JavaScript value for pointer type");
	}

	return ptr;
}

v8::Handle<v8::String> bridjs::Utils::toV8String(const char val){
	//HandleScope scope;
	char str[] = {val, '\0'};

	return v8::String::New(str);
}

bridjs::ValueWrapper::ValueWrapper(v8::Persistent<v8::Value> value){
	this->mValue = value;
}

v8::Handle<v8::Value> bridjs::ValueWrapper::getValue(){
	return this->mValue;
}

v8::Handle<v8::Value> bridjs::Utils::convertDataByType(std::shared_ptr<void> spData,const char type){
	void* pData = spData.get();

	switch(type){
		case DC_SIGCHAR_VOID:{
			return v8::Undefined();
			}
			break;
		case DC_SIGCHAR_BOOL:{
			return v8::Boolean::New(*(static_cast<DCbool*>(pData)));
			}
			break;
		case DC_SIGCHAR_UCHAR:{
			return (bridjs::Utils::toV8String(*(static_cast<DCuchar*>(pData))));
			}
			break;
		case DC_SIGCHAR_CHAR:{
			return (bridjs::Utils::toV8String(*(static_cast<DCchar*>(pData))));
			}
			break;
        case DC_SIGCHAR_SHORT:{
			return v8::Int32::New(*(static_cast<DCshort*>(pData)));
			}
			break;
		case DC_SIGCHAR_USHORT:{
			return v8::Uint32::New(*(static_cast<DCushort*>(pData)));
			}
			break;
        case DC_SIGCHAR_INT:{
			return v8::Int32::New(*(static_cast<DCint*>(pData)));
			}
			break;
		case DC_SIGCHAR_UINT:{
			return v8::Uint32::New(*(static_cast<DCuint*>(pData)));
			}
			break; 
        case DC_SIGCHAR_LONG:{
			return v8::Number::New(*(static_cast<DClong*>(pData)));
			}
			break;
		case DC_SIGCHAR_ULONG:{
			return v8::Number::New(*(static_cast<DCulong*>(pData)));
			}
			break;
		case DC_SIGCHAR_LONGLONG:{
			return v8::Number::New(static_cast<double>(*(static_cast<DClonglong*>(pData))));
			}
			break;
		case DC_SIGCHAR_ULONGLONG:{
			return v8::Number::New(static_cast<double>(*(static_cast<DCulonglong*>(pData))));
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			return v8::Number::New(static_cast<double>(*(static_cast<DCfloat*>(pData))));
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			return v8::Number::New(static_cast<double>(*(static_cast<DCdouble*>(pData))));
			}
			break;
		case DC_SIGCHAR_STRING:{
			return WRAP_STRING(*(static_cast<const char**>(pData)));
			}
			break;
        case DC_SIGCHAR_POINTER:{
			return bridjs::Utils::wrapPointer(*(static_cast<DCpointer*>(pData)));
			}
			break;
		case DC_SIGCHAR_STRUCT:{
			return v8::Exception::TypeError(v8::String::New("Not implement"));
		    }
			break;
		default:
			std::stringstream message;
			message<<"Unknown returnType: "<<type<<std::endl;

			return v8::Exception::TypeError(v8::String::New(message.str().c_str()));
		}
}


bridjs::ValueWrapper::~ValueWrapper(){
	this->mValue.Dispose();
	this->mValue.Clear();
}


