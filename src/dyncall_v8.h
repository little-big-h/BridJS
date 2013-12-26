#ifndef _DYNCALL_V8_H
#define _DYNCALL_V8_H

#include <node.h>

extern "C"
{
	#include "dyncall.h"
}

namespace bridjs{
class Dyncall {
public:
	static v8::Handle<v8::Value> newCallVM(const v8::Arguments& args);
	static v8::Handle<v8::Value> free(const v8::Arguments& args);
	static v8::Handle<v8::Value> reset(const v8::Arguments& args);
	static v8::Handle<v8::Value> mode(const v8::Arguments& args);
	static v8::Handle<v8::Value> argBool(const v8::Arguments& args);
	static v8::Handle<v8::Value> argChar(const v8::Arguments& args);
	static v8::Handle<v8::Value> argShort(const v8::Arguments& args);
	static v8::Handle<v8::Value> argInt(const v8::Arguments& args);
	static v8::Handle<v8::Value> argLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> argLongLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> argFloat(const v8::Arguments& args);
	static v8::Handle<v8::Value> argDouble(const v8::Arguments& args);
	static v8::Handle<v8::Value> argPointer(const v8::Arguments& args);
	static v8::Handle<v8::Value> argStruct(const v8::Arguments& args);

	static v8::Handle<v8::Value> callVoid(const v8::Arguments& args);
	static v8::Handle<v8::Value> callBool(const v8::Arguments& args);
	static v8::Handle<v8::Value> callChar(const v8::Arguments& args);
	static v8::Handle<v8::Value> callShort(const v8::Arguments& args);
	static v8::Handle<v8::Value> callInt(const v8::Arguments& args);
	static v8::Handle<v8::Value> callLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> callLongLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> callFloat(const v8::Arguments& args);
	static v8::Handle<v8::Value> callDouble(const v8::Arguments& args);
	static v8::Handle<v8::Value> callPointer(const v8::Arguments& args);
	static v8::Handle<v8::Value> callStruct(const v8::Arguments& args);

	static v8::Handle<v8::Value> getError(const v8::Arguments& args);

	static v8::Handle<v8::Value> newStruct(const v8::Arguments& args);
	static v8::Handle<v8::Value> structField(const v8::Arguments& args);
	static v8::Handle<v8::Value> subStruct(const v8::Arguments& args);
	static v8::Handle<v8::Value> closeStruct(const v8::Arguments& args);
	//static v8::Handle<v8::Value> structAlignment(const v8::Arguments& args);
	static v8::Handle<v8::Value> freeStruct(const v8::Arguments& args);
	static v8::Handle<v8::Value> structSize(const v8::Arguments& args);
	static v8::Handle<v8::Value> defineStruct(const v8::Arguments& args);
}; // namespace dyncall
}//bridjs
#endif  // _DYNCALL_V8_H
