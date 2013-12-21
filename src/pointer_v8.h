#pragma once

#include <node.h>

namespace bridjs{
class Pointer :public node::ObjectWrap{
public:
	static void Init(v8::Handle<v8::Object> exports);
	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> NewInstance(const void* ptr);
	static Pointer* New(const void* ptr);
	static v8::Handle<v8::Value> GetAddress(const v8::Arguments& args);
	static v8::Handle<v8::Value> IsNull(const v8::Arguments& args);
	static const void* Data(v8::Handle<v8::Object> val);

	void* getPointer();
private:
	static v8::Persistent<v8::Function> constructor;
	const void* mPtr;

	explicit Pointer(const void* ptr);
	~Pointer();
	
	
}; // namespace dyncall
}//bridjs