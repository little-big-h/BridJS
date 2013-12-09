#ifndef _DYNCALL_V8_H
#define _DYNCALL_V8_H

#include <node.h>

namespace bridjs{
class Dyncall {
public:
	static v8::Handle<v8::Value> CALL_C_DEFAULT(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_ELLIPSIS(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_ELLIPSIS_VARARGS(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_CDECL(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_WIN32_STD(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_WIN32_FAST_MS(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_WIN32_FAST_GNU(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_WIN32_THIS_MS(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_WIN32_THIS_GNU(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X64_WIN64(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X64_SYSV(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_PPC32_DARWIN(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_PPC32_OSX(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_ARM_ARM_EABI(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_ARM_THUMB_EABI(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_MIPS32_EABI(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_PPC32_SYSV(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_PPC32_LINUX(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_ARM_ARM(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_ARM_THUMB(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_MIPS32_O32(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_MIPS64_N32(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_MIPS64_N64(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_X86_PLAN9(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_SPARC32(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_C_SPARC64(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_SYS_DEFAULT(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_SYS_X86_INT80H_LINUX(const v8::Arguments& args);
	static v8::Handle<v8::Value> CALL_SYS_X86_INT80H_BSD(const v8::Arguments& args);

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
}; // namespace dyncall
}//bridjs
#endif  // _DYNCALL_V8_H
