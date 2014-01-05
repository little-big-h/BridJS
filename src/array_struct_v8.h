#pragma once

#include "pointer_v8.h"
#include "struct_v8.h"
#include <node.h>
#include <memory>
#include <node_buffer.h>
#include <vector>
#include <map>

extern "C" {
#include "dynload.h"
#include "dyncall.h"
}

namespace bridjs {

    class ArrayStruct : public bridjs::Struct {
    public:
        static void Init(v8::Handle<v8::Object> exports);
        static v8::Handle<v8::Value> New(const v8::Arguments& args);
        static v8::Handle<v8::Value> GetFieldType(const v8::Arguments& args);
        static v8::Handle<v8::Value> GetFieldOffset(const v8::Arguments& args);
        static v8::Handle<v8::Value> GetFieldCount(const v8::Arguments& args);
        static ArrayStruct* New(const char type, const size_t length, const size_t alignment = DEFAULT_ALIGNMENT);
        
        //DCCallVM* getVM() const;
        virtual const char getFieldType(const uint32_t index) const;
        virtual const size_t getFieldCount() const;
        virtual std::string getSignature();
    protected:
        char mType;
        size_t mLength;

        static v8::Persistent<v8::Function> constructor;
        static std::vector<char> mEmptyTypes;
        static std::map<uint32_t,v8::Local<v8::Object>> mEmptySubStructMap;
        
        ArrayStruct(const char type, const size_t length, const size_t aligment = DEFAULT_ALIGNMENT);
        virtual void checkRange(const uint32_t index) const;
        const size_t deriveArrayLayout(const size_t alignment);
        virtual size_t getFieldOffset(uint32_t index) const;
        virtual ~ArrayStruct();


    }; // namespace dyncall

}//bridjs