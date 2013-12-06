#ifdef _MSC_VER
#include<node.h>
#endif

#include "dyncall_v8.h"
#include "dyncallback_v8.h"
#include "dynload_v8.h"

#include <iostream>

#ifndef _MSC_VER
#include<node.h>
#endif


using namespace v8;

void init(Handle<Object> target) {
  #define EXPORT_FUNCTION(ns, name) \
    NODE_SET_METHOD(target, #name, ns::name);
  
  EXPORT_FUNCTION(dynload, loadLibrary);
  EXPORT_FUNCTION(dynload, freeLibrary);
  EXPORT_FUNCTION(dynload, findSymbol);
  EXPORT_FUNCTION(dynload, symsInit);
  EXPORT_FUNCTION(dynload, symsCleanup);
  EXPORT_FUNCTION(dynload, symsCount);
  EXPORT_FUNCTION(dynload, symsName);
  EXPORT_FUNCTION(dynload, symsNameFromValue);
  
  EXPORT_FUNCTION(dyncall, newCallVM);

  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale(""));
}

NODE_MODULE(bridjs, init);
