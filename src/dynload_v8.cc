#include "dynload_v8.h"
#include "dyncall_v8_utils.h"

#include <iostream>
#include <string>
#include <map>
#include <list>

#ifdef _MSC_VER
	#include <codecvt>
#endif

extern "C"
{
    #include "dynload.h"
	#include <uv.h>	
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

std::map<std::string, DLLib*> gLoadedLibraryMap;
uv_mutex_t gLoadLubraryMutex;


bool removeLibraryFromMap(DLLib* lib);

bool removeLibraryFromMap(DLLib* lib){
	bool found =false;

	uv_mutex_lock(&gLoadLubraryMutex);
	//gLoadLubraryMutex.lock();

	try{
		std::list<std::map<std::string, DLLib*>::iterator> removeList;

		for(std::map<std::string, DLLib*>::iterator it=gLoadedLibraryMap.begin(); it!=gLoadedLibraryMap.end() ; ++it){
			if(it->second== lib){
				removeList.push_front(it);
				found = true;
			}
			//std::cout<<"Find library: "<<lib<<std::endl;
		}

		for(std::list<std::map<std::string, DLLib*>::iterator>::iterator it=removeList.begin(); it!=removeList.end() ; ++it){
			gLoadedLibraryMap.erase(*it);
			//std::cout<<"Remove library: "<<lib<<std::endl;
		}

	}catch(...){
	  std::cerr<<"Unknown exception for cleanup library: "<<lib<<std::endl;
	}

	uv_mutex_unlock(&gLoadLubraryMutex);

	return found;
}

void Dynload::Init(v8::Handle<v8::Object> dynloadObj){
	int32_t error = uv_mutex_init(&gLoadLubraryMutex);
	if(error!=0){
		char* message = "Fail to init gLoadLubraryMutex";
		std::cerr<<message<<std::endl;

		throw std::runtime_error(message);
	}

	EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, loadLibrary);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, freeLibrary);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, findSymbol);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsInit);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsCleanup);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsCount);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsName);
    EXPORT_FUNCTION(dynloadObj,bridjs::Dynload, symsNameFromValue);
}

Handle<Value> Dynload::loadLibrary(const Arguments& args) {
  HandleScope scope;
  v8::String::Utf8Value libpath(args[0]);
  DLLib *lib = NULL;
  
  try{
	  uv_mutex_lock(&gLoadLubraryMutex);;

	  std::map<std::string, DLLib*>::iterator libIterator = gLoadedLibraryMap.find(*libpath);

	  if(libIterator !=gLoadedLibraryMap.end()){
		  lib = libIterator->second;
	  }else{
		#ifndef _MSC_VER
		  lib = dlLoadLibrary((*libpath));
		#else
		  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
		  std::u16string libPathStr = cvt.from_bytes(*libpath);//ConvertFromUtf8ToUtf16(*libpath);

		  lib = (DLLib*)LoadLibraryW((LPCWSTR)libPathStr.c_str());
		#endif
		  gLoadedLibraryMap[*libpath] = lib;
	  }
  }catch(...){
	  std::cerr<<"Unknown exception for loading library: "<<*libpath<<std::endl;
  }

  uv_mutex_unlock(&gLoadLubraryMutex);

  return scope.Close(bridjs::Utils::wrapPointer(lib));
}

Handle<Value> Dynload::freeLibrary(const Arguments& args) {
	HandleScope scope;

	GET_POINTER_ARG(DLLib, lib, args, 0);
	dlFreeLibrary(lib);
	if(!removeLibraryFromMap(lib)){
		std::stringstream message;

		message<<"Illegal library pointer: "<<lib<<std::endl;

		return  THROW_EXCEPTION(message.str().c_str());
	}

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
  pSyms = (DLSyms*)malloc(sizeof(DLSyms));

  if(pSyms!=NULL){
	  const char* base = (const char*) pLib;
	  IMAGE_DOS_HEADER*       pDOSHeader      = (IMAGE_DOS_HEADER*) base;  
	  if(pDOSHeader!=NULL){
		  IMAGE_NT_HEADERS*       pNTHeader       = (IMAGE_NT_HEADERS*) ( base + pDOSHeader->e_lfanew );  
		  IMAGE_DATA_DIRECTORY*   pExportsDataDir = &pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		  IMAGE_EXPORT_DIRECTORY* pExports        = (IMAGE_EXPORT_DIRECTORY*) (base + pExportsDataDir->VirtualAddress);  

		  pSyms->pBase  = base;
		  pSyms->pNames = (DWORD*)(base + pExports->AddressOfNames);
		  pSyms->pFuncs = (DWORD*)(base + pExports->AddressOfFunctions);
		  pSyms->pOrds  = (unsigned short*)(base + pExports->AddressOfNameOrdinals);
		  pSyms->count  = (size_t)pExports->NumberOfNames;
		  pSyms->pLib   = pLib;
	  }else{
		  throw std::exception("Fail to allocate DLSyms");
	  }
  }else{
	  throw std::exception("Fail to load library symbols");
  }
#endif

  return scope.Close(bridjs::Utils::wrapPointer(pSyms));
}

Handle<Value> Dynload::symsCleanup(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLSyms, pSyms, args, 0);
  //removeLibraryFromMap(pSyms->pLib);
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
  GET_INT32_ARG(index, args, 1);
  const char* name = dlSymsName(pSyms, index);
  return scope.Close(name ? String::New(name) : String::Empty());
}

Handle<Value> Dynload::symsNameFromValue(const Arguments& args) {
  HandleScope scope;
  GET_POINTER_ARG(DLSyms, pSyms, args, 0);
  GET_POINTER_ARG(void, value, args, 1);
  const char* name = dlSymsNameFromValue(pSyms, value);
  return scope.Close(String::New(name));
}
