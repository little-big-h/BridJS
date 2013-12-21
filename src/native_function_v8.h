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
	static v8::Handle<v8::Value> CallAsync(const v8::Arguments& args);

	static const bridjs::NativeFunction* Data(v8::Handle<v8::Value> val);
	static NativeFunction* New(void *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentType);

	//DCCallVM* getVM() const;
	void* getSymbol() const;
	const char getReturnType() const;
	const char getArgumentType(const uint32_t index) const ;
	const size_t getArgumentLength() const;
protected:
	static v8::Persistent<v8::Function> constructor;
	//DCCallVM *mpVm;
	void* mpSymbol;
	char mReturnType;
	std::vector<const char> mArgumentTypes;
	

	NativeFunction(void *pSymbol, const char returnType, 
									   const std::vector<const char> &argumentType);
	virtual ~NativeFunction();
	
	
}; // namespace dyncall

class AsyncCallTask {
private:
	DCCallVM* mpVM;
	const NativeFunction* mpNativeFunction;
	v8::Persistent<v8::Object> mpCallbackObject;
	std::shared_ptr<void> mpData;
public: 
	AsyncCallTask(DCCallVM* mpVM, const NativeFunction* mpNativeFunction,v8::Persistent<v8::Object> pCallbackObject);
	void execute();
	void done();
	v8::Handle<v8::Value> getReturnValue();
	//DCCallVM* getVM() const;
	//const NativeFunction* getNativeFunction() const;
	void setReturnData(std::shared_ptr<void> data);
	virtual ~AsyncCallTask();
	
};

}//bridjs