#pragma once

#include "pointer_v8.h"

#include <node.h>
#include <memory>
#include <node_buffer.h>
#include <vector>
#include <map>

extern "C"{
	#include "dynload.h"
	#include "dyncall.h"
}

namespace bridjs{
	class Struct :public node::ObjectWrap{
public:
	static void Init(v8::Handle<v8::Object> exports);
	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetSize(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetField(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetField(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetSignature(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetFieldType(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetFieldOffset(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetFieldCount(const v8::Arguments& args);
	static v8::Handle<v8::Value> ToString(const v8::Arguments& args);
	static Struct* New(const std::vector<char> &fieldType,std::map<uint32_t,v8::Local<v8::Object>> &subStructMap);

	//DCCallVM* getVM() const;
	static const size_t getAlignSize(size_t size, size_t alignment);
	static const size_t getFieldsSize(const std::vector<char> &fieldTypes, const size_t alignment=DEFAULT_ALIGNMENT);
	static const size_t getAlignmentSize(const char type,const size_t typeSize, const bool isFirst);
	static const size_t addPadding(size_t calculatedSize, const size_t alignment);

	virtual const char getFieldType(const uint32_t index) const ;
	virtual const size_t getFieldCount() const;
	virtual std::shared_ptr<void> getField(const uint32_t index, const void* ptr) const;
	virtual void setField(const uint32_t index, std::shared_ptr<void> pValue, void* ptr);
	const size_t getSize() const;
	virtual std::string getSignature();
	std::string toString();
protected:
	static v8::Persistent<v8::Function> constructor;
	std::vector<char> mFieldTypes;
	std::map<uint32_t,v8::Persistent<v8::Object>> mSubStructMap;
	std::vector<size_t> mOffsets;
	size_t mSize;
	size_t mAligment;

	Struct(const std::vector<char> &fieldType, std::map<uint32_t,v8::Local<v8::Object>> &subStructMap,const size_t alignment);
	virtual void checkRange(const uint32_t index) const;
	const size_t deriveLayout(const size_t alignment);
	Struct* getSubStruct(uint32_t index);
	virtual size_t getFieldOffset(uint32_t index) const;
	size_t getAlignment() const;
	virtual ~Struct();
	
	
}; // namespace dyncall

}//bridjs