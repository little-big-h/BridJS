#include "pointer_v8.h"
#include "dyncall_v8_utils.h"

using namespace bridjs;
using namespace v8;
using namespace node;

Persistent<Function> Pointer::constructor;

v8::Handle<v8::Value> ToString(const v8::Arguments& args);



void Pointer::Init(v8::Handle<v8::Object> exports){
	// Prepare constructor template
	  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	  tpl->SetClassName(String::NewSymbol("Pointer"));
	  tpl->InstanceTemplate()->SetInternalFieldCount(3);
	  // Prototype
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getAddress"),
		  FunctionTemplate::New(GetAddress)->GetFunction());
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("isNull"),
		  FunctionTemplate::New(IsNull)->GetFunction());
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
		  FunctionTemplate::New(ToString)->GetFunction());
	  constructor = Persistent<Function>::New(tpl->GetFunction());
	  exports->Set(String::NewSymbol("Pointer"), constructor);
}

v8::Handle<v8::Value> ToString(const v8::Arguments& args){
	HandleScope scope;
	Pointer* obj = ObjectWrap::Unwrap<Pointer>(args.This());
	std::stringstream ptrStream;

	ptrStream<<"Pointer: "<<std::hex<<obj->getAddress()<<std::endl;

	return scope.Close(String::New((ptrStream.str().c_str())));
}

Pointer::Pointer(const void* ptr){
	this->mPtr = ptr;
}

void* Pointer::getAddress(){
	return const_cast<void*>(this->mPtr);
}

const void* Pointer::Data(v8::Handle<v8::Object> val){
	HandleScope scope;
	Pointer* obj = ObjectWrap::Unwrap<Pointer>(val);


	return obj->getAddress();
}

Pointer* Pointer::New(const void* ptr){
	return new Pointer(ptr);
}
v8::Handle<v8::Value> Pointer::New(const v8::Arguments& args){
	HandleScope scope;

  if (args.IsConstructCall()) {
	  void* ptr;
	  Local<Value> arg0 = args[0];

	  if(arg0->IsObject() && !arg0->IsString()){
		  ptr = Utils::unwrapBufferToPointer(arg0);
	  }else{
		  GET_INT64_ARG(value, args, 0);
		  ptr = (void*)value;
	  }
	

    Pointer* obj = new Pointer(ptr);
    obj->Wrap(args.This());
    return args.This();
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}
v8::Handle<v8::Value> Pointer::NewInstance(const void* ptr){
	HandleScope scope;

    Local<Value> argv[1] = {
		Local<Value>::New(bridjs::Utils::wrapPointerToBuffer(ptr))
    };

    return scope.Close(constructor->NewInstance(1, argv));
}

v8::Handle<v8::Value> Pointer::GetAddress(const v8::Arguments& args){
	HandleScope scope;
	Pointer* obj = ObjectWrap::Unwrap<Pointer>(args.This());
	std::stringstream ptrStream;

	ptrStream<<obj->mPtr;

	return scope.Close(String::New((ptrStream.str().c_str())));
}

v8::Handle<v8::Value> Pointer::IsNull(const v8::Arguments& args){
	HandleScope scope;
	Pointer* obj = ObjectWrap::Unwrap<Pointer>(args.This());

	return scope.Close(Boolean::New(obj->mPtr==NULL));
}

Pointer::~Pointer(){
	
}