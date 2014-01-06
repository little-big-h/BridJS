/*
 * BridJS - Dynamic and blazing-fast native interop for JavaScript.
 * https://github.com/jiahansu/BridJS
 *
 * Copyright (c) 2013-2013, Olivier Chafik (http://ochafik.com/) 
 * and Jia-Han Su (https://github.com/jiahansu/BridJS)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Olivier Chafik nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY OLIVIER CHAFIK, JIA-HAN SU AND CONTRIBUTORS 
 * ``AS IS'' AND ANY * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE 
 * LIABLE FOR ANY * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
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
	  tpl->InstanceTemplate()->SetInternalFieldCount(4);
	  // Prototype
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("getAddress"),
		  FunctionTemplate::New(GetAddress)->GetFunction());
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("isNull"),
		  FunctionTemplate::New(IsNull)->GetFunction());
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
		  FunctionTemplate::New(Pointer::ToString)->GetFunction());
	  tpl->PrototypeTemplate()->Set(String::NewSymbol("slice"),
		  FunctionTemplate::New(Pointer::Slice)->GetFunction());
	  constructor = Persistent<Function>::New(tpl->GetFunction());
	  exports->Set(String::NewSymbol("Pointer"), constructor);
}

v8::Handle<v8::Value> Pointer::ToString(const v8::Arguments& args){
	HandleScope scope;
	Pointer* obj = ObjectWrap::Unwrap<Pointer>(args.This());
	std::stringstream ptrStream;

	ptrStream<<"< Pointer: "<<std::hex<<obj->getAddress()<<" >"<<std::endl;

	return scope.Close(String::New((ptrStream.str().c_str())));
}

v8::Handle<v8::Value> Pointer::Slice(const v8::Arguments& args){
	HandleScope scope;
	Pointer* obj = ObjectWrap::Unwrap<Pointer>(args.This());

	GET_INT64_ARG(start, args, 0);

	return Pointer::NewInstance(static_cast<const char*>(obj->getAddress())+start);
}

Pointer::Pointer(const void* ptr){
	this->mPtr = ptr;
}

void* Pointer::getAddress(){
	return const_cast<void*>(this->mPtr);
}

const void* Pointer::Data(v8::Handle<v8::Object> val){
	HandleScope scope;
	Pointer* obj = Pointer::Unwrap<Pointer>(val);


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
		  GET_POINTER_ARG(void,value, args, 0);
		  ptr = value;
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