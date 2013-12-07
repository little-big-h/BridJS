/*Jiahan: Must include node.h prior to v8.h in MSVC environments*/
#ifdef _MSC_VER
#include <node.h>
#endif

#include "dyncall_v8.h"
#include "dyncallback_v8.h"
#include "dynload_v8.h"

#include <iostream>
#include <math.h>

#ifndef _MSC_VER
#include <node.h>
#endif

#define EXPORT_FUNCTION(obj, ns, name) NODE_SET_METHOD(obj, #name, ns::name);

using namespace v8;

void init(Handle<Object> target) {
  Local<Object> dynloadObj = Object::New();
  Local<Object> dyncallObj = Object::New();
  Local<Object> dcbObj = Object::New();

  /*dynload*/
  target->Set(String::NewSymbol("dl"), dynloadObj);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, loadLibrary);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, freeLibrary);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, findSymbol);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsInit);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsCleanup);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsCount);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsName);
  EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsNameFromValue);
  
  /*dyncall*/
  target->Set(String::NewSymbol("dc"), dyncallObj);


    EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_DEFAULT);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_ELLIPSIS);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_ELLIPSIS_VARARGS);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_CDECL);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_WIN32_STD);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_WIN32_FAST_MS);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_WIN32_FAST_GNU);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_WIN32_THIS_MS);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_WIN32_THIS_GNU);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X64_WIN64);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X64_SYSV);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_PPC32_DARWIN);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_PPC32_OSX);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_ARM_ARM_EABI);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_ARM_THUMB_EABI);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_MIPS32_EABI);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_PPC32_SYSV);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_PPC32_LINUX);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_ARM_ARM);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_ARM_THUMB);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_MIPS32_O32);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_MIPS64_N32);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_MIPS64_N64);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_X86_PLAN9);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_SPARC32);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_C_SPARC64);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_SYS_DEFAULT);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_SYS_X86_INT80H_LINUX);
	EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, CALL_SYS_X86_INT80H_BSD);

  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, newCallVM);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, free);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, reset);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, mode);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argBool);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argShort);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argInt);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argLong);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argLongLong);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argFloat);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argDouble);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argStruct);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, argPointer);

  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callVoid);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callBool);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callChar);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callShort);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callInt);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callLong);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callLongLong);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callFloat);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callDouble);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, callStruct);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, getError);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, newStruct);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, structField);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, subStruct);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, closeStruct);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, structAlignment);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, freeStruct);

  target->Set(String::NewSymbol("dcb"), dyncallObj); 

  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale(""));
}


NODE_MODULE(bridjs, init);
