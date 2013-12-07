#include <node_buffer.h>

#include "dyncall_v8.h"
#include "dyncall_v8_utils.h"
extern "C"
{
        #include "dyncall.h"
}
#include <iostream>


using namespace v8;
using namespace bridjs;

v8::Handle<v8::Value> Dyncall::CALL_C_DEFAULT(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_DEFAULT));
}
v8::Handle<v8::Value> Dyncall::CALL_C_ELLIPSIS(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_ELLIPSIS));
}
v8::Handle<v8::Value> Dyncall::CALL_C_ELLIPSIS_VARARGS(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_ELLIPSIS_VARARGS));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_CDECL(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_CDECL));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_WIN32_STD(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_WIN32_STD));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_WIN32_FAST_MS(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_WIN32_FAST_MS));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_WIN32_FAST_GNU(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_WIN32_FAST_GNU));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_WIN32_THIS_MS(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_WIN32_THIS_MS));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_WIN32_THIS_GNU(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_WIN32_THIS_GNU));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X64_WIN64(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X64_WIN64));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X64_SYSV(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X64_SYSV));
}
v8::Handle<v8::Value> Dyncall::CALL_C_PPC32_DARWIN(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_PPC32_DARWIN));
}
v8::Handle<v8::Value> Dyncall::CALL_C_PPC32_OSX(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_PPC32_OSX));
}
v8::Handle<v8::Value> Dyncall::CALL_C_ARM_ARM_EABI(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_ARM_ARM_EABI));
}
v8::Handle<v8::Value> Dyncall::CALL_C_ARM_THUMB_EABI(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_ARM_THUMB_EABI));
}
v8::Handle<v8::Value> Dyncall::CALL_C_MIPS32_EABI(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_MIPS32_EABI));
}

v8::Handle<v8::Value> Dyncall::CALL_C_PPC32_SYSV(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_PPC32_SYSV));
}
v8::Handle<v8::Value> Dyncall::CALL_C_PPC32_LINUX(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_PPC32_LINUX));
}
v8::Handle<v8::Value> Dyncall::CALL_C_ARM_ARM(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_ARM_ARM));
}
v8::Handle<v8::Value> Dyncall::CALL_C_ARM_THUMB(const v8::Arguments& args){
		HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_ARM_THUMB));
	}
v8::Handle<v8::Value> Dyncall::CALL_C_MIPS32_O32(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_MIPS32_O32));
}
v8::Handle<v8::Value> Dyncall::CALL_C_MIPS64_N32(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_MIPS64_N32));
}
v8::Handle<v8::Value> Dyncall::CALL_C_MIPS64_N64(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_MIPS64_N64));
}
v8::Handle<v8::Value> Dyncall::CALL_C_X86_PLAN9(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_X86_PLAN9));
}
v8::Handle<v8::Value> Dyncall::CALL_C_SPARC32(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_SPARC32));
}
v8::Handle<v8::Value> Dyncall::CALL_C_SPARC64(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_C_SPARC64));
}
v8::Handle<v8::Value> Dyncall::CALL_SYS_DEFAULT(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_SYS_DEFAULT));
}
v8::Handle<v8::Value> Dyncall::CALL_SYS_X86_INT80H_LINUX(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_SYS_X86_INT80H_LINUX));
}
v8::Handle<v8::Value> Dyncall::CALL_SYS_X86_INT80H_BSD(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Integer::New(DC_CALL_SYS_X86_INT80H_BSD));
}

Handle<Value> Dyncall::newCallVM(const v8::Arguments& args) {
  HandleScope scope;
  GET_NUMBER_ARG(size, args, 0);
  DCCallVM *vm = dcNewCallVM((int)size);

  //std::cout<<(void*)vm<<std::endl;

  return scope.Close(bridjs::Utils::wrapPointer(vm));
}

v8::Handle<v8::Value> Dyncall::free(const v8::Arguments& args){
	HandleScope scope;
	
	CHECK_ARGUMENT(args,1);
	GET_POINTER_ARG(DCCallVM,vm,args,0);

	//vm = (DCCallVM*)bridjs::Utils::unwrapPointer(args[0]->ToObject());

	dcFree((DCCallVM*)vm);

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::reset(const v8::Arguments& args){
	HandleScope scope;

	CHECK_ARGUMENT(args,1);
	GET_POINTER_ARG(DCCallVM,vm,args,0);

	dcReset((DCCallVM*)vm);

	//std::cout<<(void*)vm<<std::endl;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::mode(const v8::Arguments& args){
	HandleScope scope;
	Local<Value> v;

	CHECK_ARGUMENT(args,2);
	GET_POINTER_ARG(DCCallVM,vm,args,0);
	GET_INT32_ARG(mode,args,1);

	dcMode((DCCallVM*)vm,mode);

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argBool(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argChar(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argShort(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argInt(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argLong(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argLongLong(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argFloat(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argDouble(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argPointer(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::argStruct(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Dyncall::callVoid(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callBool(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callChar(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callShort(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callInt(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callLong(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callLongLong(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callFloat(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callDouble(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callPointer(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::callStruct(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::getError(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::newStruct(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::structField(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::subStruct(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::closeStruct(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::structAlignment(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}
v8::Handle<v8::Value> Dyncall::freeStruct(const v8::Arguments& args){
	HandleScope scope;

	return scope.Close(v8::Undefined());
}