#include "native_function_v8.h"
#include "dyncall_v8_utils.h"
#include <iostream>

extern "C"{
	#include "dyncall.h"
	#include "dyncall_signature.h"
	#include <uv.h>
}

using namespace bridjs;
using namespace v8;
using namespace node;

Persistent<v8::Function> bridjs::NativeFunction::constructor;


v8::Handle<v8::Value> pushArgs(DCCallVM *vm,const bridjs::NativeFunction *nativeFunction, const v8::Arguments& args, const uint32_t offset){
	HandleScope scope;
	uint32_t i;
	for(uint32_t k=0; k<nativeFunction->getArgumentLength();++k){
		i =k+offset;
		switch(nativeFunction->getArgumentType(k)){
		case DC_SIGCHAR_BOOL:{
			GET_BOOL_ARG(val,args,i);
			dcArgBool(vm,val);
			}
			break;
		case DC_SIGCHAR_UCHAR:
		case DC_SIGCHAR_CHAR:{
			GET_CHAR_ARG(val,args,i);
			dcArgChar(vm,val);
			}
			break;
        case DC_SIGCHAR_SHORT:
		case DC_SIGCHAR_USHORT:{
			GET_INT32_ARG(val,args,i);
			dcArgShort(vm,static_cast<DCshort>(val));
			}
			break;
        case DC_SIGCHAR_INT:
		case DC_SIGCHAR_UINT:{
			GET_INT32_ARG(val,args,i);
			dcArgInt(vm,(val));
			}
			break; 
        case DC_SIGCHAR_LONG:
		case DC_SIGCHAR_ULONG:{
			GET_INT64_ARG(val,args,i);
			dcArgLong(vm,static_cast<DClong>(val));
			}
			break;
        case DC_SIGCHAR_LONGLONG:
		case DC_SIGCHAR_ULONGLONG:{
			GET_INT64_ARG(val,args,i);
			dcArgLongLong(vm,static_cast<DClonglong>(val));
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			GET_FLOAT_ARG(val,args,i);
			dcArgFloat(vm,val);
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			GET_DOUBLE_ARG(val,args,i);
			dcArgDouble(vm,(val));
		    }
			break; 
		case DC_SIGCHAR_STRING:
        case DC_SIGCHAR_POINTER:{
			GET_POINTER_ARG(void,val,args,i);
			dcArgPointer(vm,val);
		    }
			break; 
		case DC_SIGCHAR_STRUCT:{
			
			return v8::Exception::TypeError(v8::String::New("Not implement"));
		    }
			break;
		}
    }

	return scope.Close(v8::Null());
}

std::shared_ptr<void> callByType(DCCallVM *vm,const bridjs::NativeFunction *nativeFunction){
	//DCCallVM *vm = nativeFunction->getVM();
	void *pSymbol = nativeFunction->getSymbol();
	std::shared_ptr<void> data = NULL;

	switch(nativeFunction->getReturnType()){
		case DC_SIGCHAR_VOID:{
			dcCallVoid(vm, pSymbol);
			data = NULL;
			}
			break;
		case DC_SIGCHAR_BOOL:{
			data = std::shared_ptr<void>(new DCbool(dcCallBool(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_UCHAR:{
			data = std::shared_ptr<void>(new DCuchar(dcCallChar(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_CHAR:{
			data = std::shared_ptr<void>(new DCchar(dcCallChar(vm,pSymbol)));
			}
			break;
        case DC_SIGCHAR_SHORT:{
			data = std::shared_ptr<void>(new DCshort(dcCallShort(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_USHORT:{
			data = std::shared_ptr<void>(new DCushort(dcCallShort(vm,pSymbol)));
			}
			break;
        case DC_SIGCHAR_INT:{
			data = std::shared_ptr<void>(new DCint(dcCallInt(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_UINT:{
			data = std::shared_ptr<void>(new DCuint(dcCallInt(vm,pSymbol)));
			}
			break; 
        case DC_SIGCHAR_LONG:{
			data = std::shared_ptr<void>(new DClong(dcCallLong(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_ULONG:{
			data = std::shared_ptr<void>(new DCulong(dcCallLong(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_LONGLONG:{
			data = std::shared_ptr<void>(new DClonglong(dcCallLongLong(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_ULONGLONG:{
			data = std::shared_ptr<void>(new DCulonglong(dcCallLongLong(vm,pSymbol)));
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			data = std::shared_ptr<void>(new DCfloat(dcCallFloat(vm,pSymbol)));
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			data = std::shared_ptr<void>(new DCdouble(dcCallDouble(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_STRING:
        case DC_SIGCHAR_POINTER:{
			data = std::shared_ptr<void>(new DCpointer(dcCallPointer(vm,pSymbol)));
			}
			break;
		case DC_SIGCHAR_STRUCT:{
			std::cerr<<"Not implement"<<std::endl;
			data = std::shared_ptr<void*>(NULL);
		    }
			break;

		default:
			std::stringstream message;
			message<<"Unknown returnType: "<<nativeFunction->getReturnType()<<std::endl;
			throw std::runtime_error(message.str());
			//return v8::Exception::TypeError(v8::String::New(message.str().c_str()));
		}

	return data;
}

v8::Handle<v8::Value> convertDataByType(std::shared_ptr<void> spData,const char type){
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
		case DC_SIGCHAR_STRING:
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

void executeCallAsync(uv_work_t *req){
	AsyncCallTask *pAsyncCall = static_cast<AsyncCallTask*>(req->data);
	//std::cout<<"Got it"<<std::endl;
	if(pAsyncCall!=NULL){
		pAsyncCall->execute();
	}else{
		std::cerr<<"Fail to cast data pointer to AsyncCall"<<std::endl;
	}

	//std::cout<<"Got it"<<std::endl;
}

void afterCallAsync(uv_work_t *req){
	AsyncCallTask *pAsyncCall = static_cast<AsyncCallTask*>(req->data);

	if(pAsyncCall!=NULL){
		pAsyncCall->done();
		delete req->data;
		req->data = NULL;
	}else{
		std::cerr<<"Fail to cast data pointer to AsyncCall"<<std::endl;
	}

	//std::cout<<"Got it2"<<std::endl;
}

void bridjs::NativeFunction::Init(v8::Handle<v8::Object> exports){
	// Prepare constructor template
	  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	 

	  tpl->SetClassName(String::NewSymbol("NativeFunction"));
	  tpl->InstanceTemplate()->SetInternalFieldCount(5);
	  // Prototype
	  /*
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getVM"),
		  FunctionTemplate::New(GetVM)->GetFunction(), ReadOnly);*/
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getSymbolName"),
		  FunctionTemplate::New(GetSymbol)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getReturnType"),
		  FunctionTemplate::New(GetReturnType)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getArgumentType"),
		  FunctionTemplate::New(GetArgumentType)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getArgumentsLength"),
		  FunctionTemplate::New(GetArgumentsLength)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("call"),
		  FunctionTemplate::New(Call)->GetFunction(), ReadOnly),
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("callAsync"),
		  FunctionTemplate::New(CallAsync)->GetFunction(), ReadOnly);

	  constructor = Persistent<Function>::New(tpl->GetFunction());

	exports->Set(String::NewSymbol("NativeFunction"), constructor);
}



const bridjs::NativeFunction* bridjs::NativeFunction::Data(v8::Handle<v8::Value> val){
	HandleScope scope;
	NativeFunction* obj;
	
	if(val->IsObject()){
		obj = ObjectWrap::Unwrap<NativeFunction>(val->ToObject());
	}else{
		obj = NULL;
	}

	return obj;
}

bridjs::NativeFunction* bridjs::NativeFunction::New(void *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentTypes){
	return new bridjs::NativeFunction(pSymbol,returnType, argumentTypes);
}
v8::Handle<v8::Value> bridjs::NativeFunction::New(const v8::Arguments& args){
	HandleScope scope;

  if (args.IsConstructCall()) {
	std::vector<const char> argumentTypes;
	//GET_POINTER_ARG(DCCallVM,pVM,args,0);
	GET_POINTER_ARG(DLSyms,pSymbol,args,0);
	GET_CHAR_ARG(returnType,args,1); 


	for(int32_t i = 2;i<args.Length();++i){
		GET_CHAR_ARG(type,args,i);
		argumentTypes.push_back(type);
	}

    NativeFunction* obj = new NativeFunction( pSymbol,returnType, argumentTypes);
    obj->Wrap(args.This());
    return args.This();
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}
/*
v8::Handle<v8::Value> bridjs::Signature::NewInstance(const void* ptr){
	HandleScope scope;

    Local<Value> argv[1] = {
		Local<Value>::New(bridjs::Utils::wrapPointerToBuffer(ptr))
    };

    return scope.Close(constructor->NewInstance(1, argv));
}*/

v8::Handle<v8::Value> bridjs::NativeFunction::GetReturnType(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<NativeFunction>(args.This());

	return scope.Close(bridjs::Utils::toV8String(obj->getReturnType()));
}

v8::Handle<v8::Value> bridjs::NativeFunction::GetArgumentType(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<bridjs::NativeFunction>(args.This());
	v8::Handle<v8::Value> value;
	GET_INT32_ARG(index,args,0);

	try{
		value =  scope.Close(bridjs::Utils::toV8String(obj->getArgumentType(index)));
	}catch(std::out_of_range& e){
		value = THROW_EXCEPTION(e.what());
	}

	return value;
}


v8::Handle<v8::Value> bridjs::NativeFunction::GetArgumentsLength(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<NativeFunction>(args.This());

	return scope.Close(v8::Int32::New(static_cast<int32_t>(obj->getArgumentLength())));
}
/*
v8::Handle<v8::Value> bridjs::NativeFunction::GetVM(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<NativeFunction>(args.This());

	return scope.Close(bridjs::Utils::wrapPointer(obj->getVM()));
}*/
v8::Handle<v8::Value> bridjs::NativeFunction::GetSymbol(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<NativeFunction>(args.This());

	return scope.Close(bridjs::Utils::wrapPointer(obj->getSymbol()));
}

v8::Handle<v8::Value> bridjs::NativeFunction::Call(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* nativeFunction = ObjectWrap::Unwrap<NativeFunction>(args.This());
	GET_POINTER_ARG(DCCallVM,vm,args,0);

	if(nativeFunction!=NULL){
		Handle<Value> error;
		try{
			dcReset(vm);
			error = pushArgs(vm,nativeFunction, args,1);

			if(error->IsNull()){
				Handle<Value> returnValue =convertDataByType(callByType(vm,nativeFunction), nativeFunction->getReturnType());
				DCint errorCode = dcGetError(vm);

				if(errorCode!=0){
					std::stringstream message;
					message<<"Dyncall error, errorCode: "<<errorCode<<std::endl;

					return v8::Exception::Error(v8::String::New(message.str().c_str()));
				}else{
					return returnValue;
				}
			}else{
				return error;
			}
		}catch(std::runtime_error e){
			return THROW_EXCEPTION(e.what());
		}catch(std::out_of_range& e){
			return THROW_EXCEPTION(e.what());
		}
	}else{
		return v8::Exception::TypeError(v8::String::New("This must be NativeFunction's instance"));
	}
}

v8::Handle<v8::Value> bridjs::NativeFunction::CallAsync(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* nativeFunction = ObjectWrap::Unwrap<NativeFunction>(args.This());
	GET_POINTER_ARG(DCCallVM,vm,args,0);
	
	if(nativeFunction!=NULL){
		Local<v8::Value> callbackObject = args[args.Length()-1];

		if(callbackObject->IsObject()){

			Handle<Value> error;
			try{
				dcReset(vm);
				error = pushArgs(vm,nativeFunction, args,1);

				if(error->IsNull()){
					uv_work_t *req = new uv_work_t;
					req->data = new bridjs::AsyncCallTask(vm,nativeFunction,Persistent<Object>::New(callbackObject->ToObject()));
					uv_queue_work(uv_default_loop(),req,executeCallAsync,(uv_after_work_cb)afterCallAsync);
					/*
					Handle<Value> returnValue = callByType(vm,nativeFunction);
					DCint errorCode = dcGetError(vm);

					if(errorCode!=0){
						std::stringstream message;
						message<<"Dyncall error, errorCode: "<<errorCode<<std::endl;

						return v8::Exception::Error(v8::String::New(message.str().c_str()));
					}else{
						return returnValue;
					}*/
					return scope.Close(v8::Undefined());
				}else{
					return error;
				}
			}catch(std::out_of_range& e){
				return THROW_EXCEPTION(e.what());
			}
		}else{
			std::stringstream message;

			message<<"Last argument must a CallbackObject's instance: "<<(*v8::String::Utf8Value(callbackObject->ToString()))<<std::endl;

			return v8::Exception::TypeError(v8::String::New(message.str().c_str()));
		}
	}else{
		std::stringstream message;

		message<<"Last argument must a CallbackObject's instance: "<<(*v8::String::Utf8Value(args.This()->ToString()))<<std::endl;

		return v8::Exception::TypeError(v8::String::New("This must be NativeFunction's instance"));
	}
}


bridjs::NativeFunction::NativeFunction(void *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentTypes){
	//this->mpVm = (DCCallVM*)pVm;
	this->mpSymbol = pSymbol;
	this->mReturnType  =returnType;
	this->mArgumentTypes = argumentTypes;
	this->mArgumentTypes.shrink_to_fit();
}

const char bridjs::NativeFunction::getReturnType() const{
	return this->mReturnType;	
}

const char bridjs::NativeFunction::getArgumentType(const uint32_t index)  const{
	//std::cout<<this->mArgumentTypes<<std::endl;
	if(index<this->mArgumentTypes.size()){
		return mArgumentTypes.at(index);
	}else{
		std::stringstream message;

		message<<"Index: "<<index<<" was out of boundary, size = "<<this->mArgumentTypes.size();

		throw std::out_of_range(message.str().c_str()); 
	}
}

const size_t bridjs::NativeFunction::getArgumentLength() const{
	
	return this->mArgumentTypes.size();	
}
/*
DCCallVM* bridjs::NativeFunction::getVM() const{
	return this->mpVm;
}*/
void* bridjs::NativeFunction::getSymbol() const{
	return this->mpSymbol;
}

bridjs::NativeFunction::~NativeFunction(){
	
}

AsyncCallTask::AsyncCallTask(DCCallVM* pVM, const NativeFunction* pNativeFunction, v8::Persistent<v8::Object> pCallbackObject){
	this->mpVM = pVM;
	this->mpNativeFunction = pNativeFunction;
	this->mpCallbackObject = pCallbackObject;
}

v8::Handle<Value> AsyncCallTask::getReturnValue(){
	return v8::Null();
}
/*
DCCallVM* AsyncCallTask::getVM() const{
	return this->mpVM;
}
const NativeFunction* AsyncCallTask::getNativeFunction() const{
	return this->mpNativeFunction;
}*/

void AsyncCallTask::execute(){
	this->mpData = callByType(this->mpVM,this->mpNativeFunction);
}

void AsyncCallTask::done(){
	HandleScope scope;
	int32_t error = dcGetError(this->mpVM);

	if(error==0){
		v8::Local<Value> onDoneValue = this->mpCallbackObject->GetRealNamedProperty(v8::String::New("onDone"));

		if(onDoneValue->IsFunction()){
			v8::Local<v8::Function> onDoneFunction = v8::Local<Function>::Cast(onDoneValue);
			Handle<Value> argv[] = { bridjs::Utils::wrapPointer(this->mpVM), convertDataByType(this->mpData,this->mpNativeFunction->getReturnType())};

			onDoneFunction->Call(this->mpCallbackObject,2, argv);
		}else{
			std::cerr<<"Illegal callback object: "<<(*v8::String::Utf8Value(this->mpCallbackObject->ToString()))<<std::endl;
		}
	}else{
		v8::Local<Value> onErrorValue = this->mpCallbackObject->GetRealNamedProperty(v8::String::New("onError"));

		if(onErrorValue ->IsFunction()){
			v8::Local<v8::Function> onErrorFunction = v8::Local<Function>::Cast(onErrorValue);
			std::stringstream message;
			message<<"Dyncall error, errorCode: "<<error;

			Handle<Value> argv[] = { bridjs::Utils::wrapPointer(this->mpVM), v8::Exception::Error(v8::String::New(message.str().c_str()))};

			onErrorFunction->Call(this->mpCallbackObject,2, argv);
		}else{
			std::cerr<<"Illegal callback object: "<<(*v8::String::Utf8Value(this->mpCallbackObject->ToString()))<<std::endl;
		}
	}
}

AsyncCallTask::~AsyncCallTask(){
	
}