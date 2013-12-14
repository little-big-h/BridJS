#include "native_function_v8.h"
#include "dyncall_v8_utils.h"
#include <iostream>

extern "C"{
	#include "dyncall.h"
	#include "dyncall_signature.h"
}

using namespace bridjs;
using namespace v8;
using namespace node;

Persistent<v8::Function> bridjs::NativeFunction::constructor;


v8::Handle<v8::Value> pushArgs(const bridjs::NativeFunction *nativeFunction, const v8::Arguments& args){
	HandleScope scope;
	DCCallVM *vm = nativeFunction->getVM();

	for(uint32_t i=0; i<nativeFunction->getArgumentLength();++i){
		switch(nativeFunction->getArgumentType(i)){
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

v8::Handle<v8::Value> callByType(const bridjs::NativeFunction *nativeFunction){
	DCCallVM *vm = nativeFunction->getVM();
	DLSyms *pSymbol = nativeFunction->getSymbol();

	switch(nativeFunction->getReturnType()){
		case DC_SIGCHAR_VOID:{
			dcCallVoid(vm,pSymbol);
			return v8::Undefined();
			}
			break;
		case DC_SIGCHAR_BOOL:{
			return v8::Boolean::New(dcCallBool(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_UCHAR:
		case DC_SIGCHAR_CHAR:{
			return (bridjs::Utils::toV8String(dcCallChar(vm,pSymbol)));
			}
			break;
        case DC_SIGCHAR_SHORT:{
			return v8::Int32::New(dcCallInt(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_USHORT:{
			return v8::Uint32::New(dcCallInt(vm,pSymbol));
			}
			break;
        case DC_SIGCHAR_INT:{
			return v8::Int32::New(dcCallInt(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_UINT:{
			return v8::Uint32::New(dcCallInt(vm,pSymbol));
			}
			break; 
        case DC_SIGCHAR_LONG:{
			return v8::Number::New(dcCallLong(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_ULONG:{
			return v8::Number::New(dcCallLong(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_LONGLONG:{
			return v8::Number::New(dcCallLongLong(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_ULONGLONG:{
			return v8::Number::New(dcCallLongLong(vm,pSymbol));
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			return v8::Number::New(dcCallFloat(vm,pSymbol));
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			return v8::Number::New(dcCallDouble(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_STRING:
        case DC_SIGCHAR_POINTER:{
			return bridjs::Utils::wrapPointer(dcCallPointer(vm,pSymbol));
			}
			break;
		case DC_SIGCHAR_STRUCT:{
			return v8::Exception::TypeError(v8::String::New("Not implement"));
		    }
			break;

		default:
			std::stringstream message;
			message<<"Unknown returnType: "<<nativeFunction->getReturnType()<<std::endl;

			return v8::Exception::TypeError(v8::String::New(message.str().c_str()));
		}
}

void bridjs::NativeFunction::Init(v8::Handle<v8::Object> exports){
	// Prepare constructor template
	  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	 

	  tpl->SetClassName(String::NewSymbol("NativeFunction"));
	  tpl->InstanceTemplate()->SetInternalFieldCount(5);
	  // Prototype
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getVM"),
		  FunctionTemplate::New(GetVM)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getSymbolName"),
		  FunctionTemplate::New(GetSymbol)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getReturnType"),
		  FunctionTemplate::New(GetReturnType)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getArgumentType"),
		  FunctionTemplate::New(GetArgumentType)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getArgumentsLength"),
		  FunctionTemplate::New(GetArgumentsLength)->GetFunction(), ReadOnly);
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("call"),
		  FunctionTemplate::New(Call)->GetFunction(), ReadOnly);

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

bridjs::NativeFunction* bridjs::NativeFunction::New(DCCallVM *pVm,DLSyms *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentTypes){
	return new bridjs::NativeFunction(pVm,pSymbol,returnType, argumentTypes);
}
v8::Handle<v8::Value> bridjs::NativeFunction::New(const v8::Arguments& args){
	HandleScope scope;

  if (args.IsConstructCall()) {
	std::vector<const char> argumentTypes;
	GET_POINTER_ARG(DCCallVM,pVM,args,0);
	GET_POINTER_ARG(DLSyms,pSymbol,args,1);
	GET_CHAR_ARG(returnType,args,2); 


	for(int32_t i = 3;i<args.Length();++i){
		GET_CHAR_ARG(type,args,i);
		argumentTypes.push_back(type);
	}

    NativeFunction* obj = new NativeFunction( pVM, pSymbol,returnType, argumentTypes);
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

v8::Handle<v8::Value> bridjs::NativeFunction::GetVM(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<NativeFunction>(args.This());

	return scope.Close(bridjs::Utils::wrapPointer(obj->getVM()));
}
v8::Handle<v8::Value> bridjs::NativeFunction::GetSymbol(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* obj = ObjectWrap::Unwrap<NativeFunction>(args.This());

	return scope.Close(bridjs::Utils::wrapPointer(obj->getSymbol()));
}

v8::Handle<v8::Value> bridjs::NativeFunction::Call(const v8::Arguments& args){
	HandleScope scope;
	bridjs::NativeFunction* nativeFunction = ObjectWrap::Unwrap<NativeFunction>(args.This());
	DCCallVM *vm = nativeFunction->getVM();

	if(nativeFunction!=NULL){
		Handle<Value> error;

		dcReset(vm);
		error = pushArgs(nativeFunction, args);

		if(error->IsNull()){
			Handle<Value> returnValue = callByType(nativeFunction);
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

	}else{
		return v8::Exception::TypeError(v8::String::New("This must be NativeFunction's instance"));
	}
}


bridjs::NativeFunction::NativeFunction(DCCallVM *pVm,DLSyms *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentTypes){
	this->mpVm = (DCCallVM*)pVm;
	this->mpSymbol = pSymbol;
	this->mReturnType  =returnType;
	this->mArgumentTypes = argumentTypes;
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

DCCallVM* bridjs::NativeFunction::getVM() const{
	return this->mpVm;
}
DLSyms* bridjs::NativeFunction::getSymbol() const{
	return this->mpSymbol;
}

bridjs::NativeFunction::~NativeFunction(){
	
}