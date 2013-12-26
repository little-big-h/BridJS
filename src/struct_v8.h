#pragma once

#include "pointer_v8.h"

#include <node.h>
#include <memory>
#include <node_buffer.h>
#include <vector>

extern "C"{
	#include "dynload.h"
	#include "dyncall.h"
}

namespace bridjs{
	class Struct :public node::ObjectWrap{
public:
	static void Init(v8::Handle<v8::Object> exports);
	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	//static v8::Handle<v8::Value> NewInstance(const void* ptr);
	static v8::Handle<v8::Value> GetFieldType(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetFieldCount(const v8::Arguments& args);
	static Struct* New(const std::vector<const char> &fieldType);

	//DCCallVM* getVM() const;
	const char getFieldType(const uint32_t index) const ;
	const size_t getFieldCount() const;
	std::shared_ptr<void> getField(const uint32_t index, const void* ptr) const;
	void setField(const uint32_t index, std::shared_ptr<void> pValue, void* ptr);
	const size_t getSize() const;
protected:
	static v8::Persistent<v8::Function> constructor;
	std::vector<const char> mFieldTypes;
	std::shared_ptr<char> mBuffer;
	std::vector<size_t> mOffsets;
	size_t mSize;

	Struct(const std::vector<const char> &fieldType,const size_t alignmen);
	void checkRange(const uint32_t index) const;
	const size_t deriveLayout(const size_t alignment);
	virtual ~Struct();
	
	
}; // namespace dyncall

}//bridjs