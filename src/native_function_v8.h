#pragma once

#include <node.h>
#include <vector>
#include <memory>
#include <exception>

extern "C"{
	#include "dynload.h"
	#include "dyncall.h"
}

namespace bridjs{
class NativeFunction :public node::ObjectWrap{
public:
	static void Init(v8::Handle<v8::Object> exports);
	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	//static v8::Handle<v8::Value> NewInstance(const void* ptr);
	static v8::Handle<v8::Value> GetReturnType(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetArgumentType(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetArgumentsLength(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetVM(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetSymbol(const v8::Arguments& args);
	static v8::Handle<v8::Value> Call(const v8::Arguments& args);

	static const bridjs::NativeFunction* Data(v8::Handle<v8::Value> val);
	static NativeFunction* New(DCCallVM *pVm,DLSyms *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentType);

	DCCallVM* getVM() const;
	DLSyms* getSymbol() const;
	const char getReturnType() const;
	const char getArgumentType(const uint32_t index) const ;
	const size_t getArgumentLength() const;
private:
	static v8::Persistent<v8::Function> constructor;
	DCCallVM *mpVm;
	DLSyms* mpSymbol;
	char mReturnType;
	std::vector<const char> mArgumentTypes;
	

	NativeFunction(DCCallVM *pVm,DLSyms *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentType);
	virtual ~NativeFunction();
	
	
}; // namespace dyncall
}//bridjs