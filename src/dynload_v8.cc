#include "dynload_v8.h"
#include "dyncall_v8_utils.h"

#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
	#include <codecvt>
#endif

extern "C"
{
    #include "dynload.h"
#ifdef _MSC_VER
	#include <Windows.h>
#endif
}
using namespace v8;
using namespace bridjs;

struct DLSyms_
{
  DLLib*                pLib;
  const char*           pBase;
  const DWORD*          pNames;
  const DWORD*          pFuncs;
  const unsigned short* pOrds;
  size_t                count;
};

/*
std::string ConvertFromUtf16ToUtf8(const std::wstring& wstr)
{
    std::string convertedString;
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
    if(requiredSize > 0)
    {
        std::vector<char> buffer(requiredSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
}
 
std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
{
    std::wstring convertedString;
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    if(requiredSize > 0)
    {
        std::vector<wchar_t> buffer(requiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
 
    return convertedString;
}*/


Handle<Value> Dynload::loadLibrary(const Arguments& args) {
  HandleScope scope;
  v8::String::Utf8Value libpath(args[0]);
  DLLib *lib = NULL;
  
#ifndef _MSC_VER
  lib = dlLoadLibrary((*libpath));
#else
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
  std::u16string libPathStr = cvt.from_bytes(*libpath);//ConvertFromUtf8ToUtf16(*libpath);

  lib = (DLLib*)LoadLibraryW((LPCWSTR)libPathStr.c_str());
#endif
  //std::wcout<<(*libpath)<<", lib: "<<lib<<std::endl;
 
  return scope.Close(bridjs::Utils::wrapPointer(lib));
}

Handle<Value> Dynload::freeLibrary(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLLib, lib, args, 0);
  dlFreeLibrary(lib);
  return scope.Close(Undefined());
}

Handle<Value> Dynload::findSymbol(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLLib, lib, args, 0);
  GET_ASCII_STRING_ARG(name, args, 1);
  void* symbol = dlFindSymbol(lib, name);
  return scope.Close(bridjs::Utils::wrapPointer(symbol));
}

Handle<Value> Dynload::symsInit(const Arguments& args) {
  HandleScope scope;
  v8::String::Utf8Value libpath(args[0]);
  DLSyms *pSyms = NULL;

#ifndef _MSC_VER
	pSyms = dlSymsInit(*libpath);
#else
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
  std::u16string libPathStr = cvt.from_bytes(*libpath);//ConvertFromUtf8ToUtf16(*libpath);

  DLLib* pLib = (DLLib*)LoadLibraryW((LPCWSTR)libPathStr.c_str());

  //std::wcout<<(*libpath)<<", lib: "<<pLib<<std::endl;

  pSyms = (DLSyms*)malloc(sizeof(DLSyms));
  const char* base = (const char*) pLib;
  IMAGE_DOS_HEADER*       pDOSHeader      = (IMAGE_DOS_HEADER*) base;  
  IMAGE_NT_HEADERS*       pNTHeader       = (IMAGE_NT_HEADERS*) ( base + pDOSHeader->e_lfanew );  
  IMAGE_DATA_DIRECTORY*   pExportsDataDir = &pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
  IMAGE_EXPORT_DIRECTORY* pExports        = (IMAGE_EXPORT_DIRECTORY*) (base + pExportsDataDir->VirtualAddress);  

  pSyms->pBase  = base;
  pSyms->pNames = (DWORD*)(base + pExports->AddressOfNames);
  pSyms->pFuncs = (DWORD*)(base + pExports->AddressOfFunctions);
  pSyms->pOrds  = (unsigned short*)(base + pExports->AddressOfNameOrdinals);
  pSyms->count  = (size_t)pExports->NumberOfNames;
  pSyms->pLib   = pLib;
#endif

  return scope.Close(bridjs::Utils::wrapPointer(pSyms));
}

Handle<Value> Dynload::symsCleanup(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLSyms, pSyms, args, 0);
  dlSymsCleanup(pSyms);
  return scope.Close(Undefined());
}

Handle<Value> Dynload::symsCount(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLSyms, pSyms, args, 0);
  int count = dlSymsCount(pSyms);
  return scope.Close(Number::New(count));
}

Handle<Value> Dynload::symsName(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLSyms, pSyms, args, 0);
  GET_NUMBER_ARG(index, args, 1);
  const char* name = dlSymsName(pSyms, (int)index);
  return scope.Close(name ? String::New(name) : String::Empty());
}

Handle<Value> Dynload::symsNameFromValue(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLSyms, pSyms, args, 0);
  GET_POINTER_ARG(void, value, args, 1);
  const char* name = dlSymsNameFromValue(pSyms, value);
  return scope.Close(String::New(name));
}
