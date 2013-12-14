/*Jiahan: Must include node.h prior to v8.h in MSVC build environments*/
#ifdef _MSC_VER
#include <node.h>
#endif

#include "dyncall_v8.h"
#include "dyncallback_v8.h"
#include "dynload_v8.h"
#include "pointer_v8.h"
#include "native_function_v8.h"
#include "dyncall_v8_utils.h"

#include <iostream>
#include <math.h>

extern "C"
{
	#include "dyncall_signature.h"
}

#ifndef _MSC_VER
#include <node.h>
#endif

#define EXPORT_FUNCTION(obj, ns, name) NODE_SET_METHOD(obj, #name, ns::name);

using namespace v8;

void getSignedAndUnsinedTypeFromTypeSize(const uint32_t size, char &signedType, char &unsignedType){
	switch(size){
	case sizeof(DCchar):
		unsignedType = DC_SIGCHAR_UCHAR;
		signedType = DC_SIGCHAR_CHAR;
		break;	
	case sizeof(DCshort):
		unsignedType = DC_SIGCHAR_USHORT;
		signedType = DC_SIGCHAR_SHORT;
		break;
    case sizeof(DCint):
		unsignedType = DC_SIGCHAR_UINT;
		signedType = DC_SIGCHAR_INT;
		break;
		break;
    case sizeof(DClonglong):
		unsignedType = DC_SIGCHAR_ULONGLONG;
		signedType = DC_SIGCHAR_LONGLONG;
		break;
	default:
		if(size==sizeof(DClong)){
			unsignedType = DC_SIGCHAR_ULONG;
			signedType = DC_SIGCHAR_LONG;
		}else{
			unsignedType = signedType = DC_SIGCHAR_VOID;
		}
	}
}

void init(Handle<Object> target) {
  Local<Object> dynloadObj = Object::New();
  Local<Object> dyncallObj = Object::New();
  Local<Object> dcbObj = Object::New();
  Local<Object> signatureObj = Object::New();
   char signedType = DC_SIGCHAR_VOID, unsignedType = DC_SIGCHAR_VOID;

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

  dyncallObj->Set(String::NewSymbol("CALL_C_DEFAULT"),
		  v8::Int32::New(DC_CALL_C_DEFAULT),ReadOnly);
  dyncallObj->Set(String::NewSymbol("CALL_C_ELLIPSIS"),
		  v8::Int32::New(DC_CALL_C_ELLIPSIS),ReadOnly);
  dyncallObj->Set(String::NewSymbol("CALL_C_ELLIPSIS_VARARGS"),
		  v8::Int32::New(DC_CALL_C_ELLIPSIS_VARARGS),ReadOnly);
   dyncallObj->Set(String::NewSymbol("CALL_C_CDECL"),
		  v8::Int32::New(DC_CALL_C_X86_CDECL),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X86_WIN32_STD"),
		  v8::Int32::New(DC_CALL_C_X86_WIN32_STD),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X86_WIN32_FAST_MS"),
		  v8::Int32::New(DC_CALL_C_X86_WIN32_FAST_MS),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X86_WIN32_FAST_GNU"),
		  v8::Int32::New(DC_CALL_C_X86_WIN32_FAST_GNU),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X86_WIN32_THIS_MS"),
		  v8::Int32::New(DC_CALL_C_X86_WIN32_THIS_MS),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X86_WIN32_THIS_GNU"),
		  v8::Int32::New(DC_CALL_C_X86_WIN32_THIS_GNU),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X64_WIN64"),
		  v8::Int32::New(DC_CALL_C_X64_WIN64),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X64_SYSV"),
		  v8::Int32::New(DC_CALL_C_X64_SYSV),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_PPC32_DARWIN"),
		  v8::Int32::New(DC_CALL_C_PPC32_DARWIN),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_PPC32_OSX"),
		  v8::Int32::New(DC_CALL_C_PPC32_OSX),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_ARM_ARM_EABI"),
		  v8::Int32::New(DC_CALL_C_ARM_ARM_EABI),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_ARM_THUMB_EABI"),
		  v8::Int32::New(DC_CALL_C_ARM_THUMB_EABI),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_MIPS32_EABI"),
		  v8::Int32::New(DC_CALL_C_MIPS32_EABI),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_PPC32_SYSV"),
		  v8::Int32::New(DC_CALL_C_PPC32_SYSV),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_PPC32_LINUX"),
		  v8::Int32::New(DC_CALL_C_PPC32_LINUX),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_ARM_ARM"),
		  v8::Int32::New(DC_CALL_C_ARM_ARM),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_ARM_THUMB"),
		  v8::Int32::New(DC_CALL_C_ARM_THUMB),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_MIPS32_O32"),
		  v8::Int32::New(DC_CALL_C_MIPS32_O32),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_MIPS64_N32"),
		  v8::Int32::New(DC_CALL_C_MIPS64_N32),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_MIPS64_N64"),
		  v8::Int32::New(DC_CALL_C_MIPS64_N64),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_X86_PLAN9"),
		  v8::Int32::New(DC_CALL_C_X86_PLAN9),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_SPARC32"),
		  v8::Int32::New(DC_CALL_C_SPARC32),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_C_SPARC64"),
		  v8::Int32::New(DC_CALL_C_SPARC64),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_SYS_DEFAULT"),
		  v8::Int32::New(DC_CALL_SYS_DEFAULT),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_SYS_X86_INT80H_LINUX"),
		  v8::Int32::New(DC_CALL_SYS_X86_INT80H_LINUX),ReadOnly);
	dyncallObj->Set(String::NewSymbol("CALL_SYS_X86_INT80H_BSD"),
		  v8::Int32::New(DC_CALL_SYS_X86_INT80H_BSD),ReadOnly);
	dyncallObj->Set(String::NewSymbol("Signature"),
		signatureObj,ReadOnly);
	/*Signature*/
	signatureObj->Set(String::NewSymbol("VOID_TYPE"),
		  bridjs::Utils::toV8String( DC_SIGCHAR_VOID),ReadOnly);
	  signatureObj->Set(String::NewSymbol("BOOL_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_BOOL), ReadOnly);
	   signatureObj->Set(String::NewSymbol("UCHAR_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_UCHAR), ReadOnly);
	  signatureObj->Set(String::NewSymbol("CHAR_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_CHAR), ReadOnly);
	  signatureObj->Set(String::NewSymbol("SHORT_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_SHORT), ReadOnly);
	  signatureObj->Set(String::NewSymbol("UCHAR_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_USHORT), ReadOnly);
	  signatureObj->Set(String::NewSymbol("INT_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_INT), ReadOnly);
	  signatureObj->Set(String::NewSymbol("UINT_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_UINT), ReadOnly);
	  signatureObj->Set(String::NewSymbol("LONG_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_LONG), ReadOnly);
	   signatureObj->Set(String::NewSymbol("ULONG_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_ULONG), ReadOnly);
	 signatureObj->Set(String::NewSymbol("LONGLONG_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_LONGLONG), ReadOnly);
	 signatureObj->Set(String::NewSymbol("ULONG_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_ULONG), ReadOnly);
	 signatureObj->Set(String::NewSymbol("DOUBLE_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_DOUBLE), ReadOnly);
	 signatureObj->Set(String::NewSymbol("POINTER_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_POINTER), ReadOnly);
	signatureObj->Set(String::NewSymbol("STRING_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_STRING), ReadOnly);
	signatureObj->Set(String::NewSymbol("STRUCT_TYPE"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_STRUCT), ReadOnly);
	signatureObj->Set(String::NewSymbol("ENDARG"),
		  bridjs::Utils::toV8String(DC_SIGCHAR_ENDARG), ReadOnly);
	/*INT8*/
	getSignedAndUnsinedTypeFromTypeSize(sizeof(int8_t), signedType, unsignedType);

	signatureObj->Set(String::NewSymbol("INT8_TYPE"),
		  bridjs::Utils::toV8String(signedType), ReadOnly);
	signatureObj->Set(String::NewSymbol("UINT8_TYPE"),
		  bridjs::Utils::toV8String(unsignedType), ReadOnly);

	/*INT16*/
	getSignedAndUnsinedTypeFromTypeSize(sizeof(int16_t), signedType, unsignedType);
	signatureObj->Set(String::NewSymbol("INT16_TYPE"),
		  bridjs::Utils::toV8String(signedType), ReadOnly);
	signatureObj->Set(String::NewSymbol("UINT16_TYPE"),
		  bridjs::Utils::toV8String(unsignedType), ReadOnly);
	
	/*INT32*/
	getSignedAndUnsinedTypeFromTypeSize(sizeof(int32_t), signedType, unsignedType);
	//char type[] = {signedType,'\0'};
	//std::cout<<"fefe "<<*(v8::String::Utf8Value(bridjs::Utils::toV8String(signedType)))<<std::endl;
	signatureObj->Set(String::NewSymbol("INT32_TYPE"),
		bridjs::Utils::toV8String(signedType), ReadOnly);
	signatureObj->Set(String::NewSymbol("UINT32_TYPE"),
		  bridjs::Utils::toV8String(unsignedType), ReadOnly);

	/*INT64*/
	getSignedAndUnsinedTypeFromTypeSize(sizeof(int64_t), signedType, unsignedType);
	signatureObj->Set(String::NewSymbol("INT64_TYPE"),
		  bridjs::Utils::toV8String(signedType), ReadOnly);
	signatureObj->Set(String::NewSymbol("UINT64_TYPE"),
		  bridjs::Utils::toV8String(unsignedType), ReadOnly);

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
  //EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, structAlignment);
  EXPORT_FUNCTION(dyncallObj,bridjs::Dyncall, freeStruct);

  target->Set(String::NewSymbol("dcb"), dyncallObj); 

  bridjs::Pointer::Init(dyncallObj);
  bridjs::NativeFunction::Init(dyncallObj);

  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale(""));
}


NODE_MODULE(bridjs, init);

extern "C"{
	__declspec(dllexport) double testMultiplyFunction(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e){
		return w*x*y*z*e;
	}
}
