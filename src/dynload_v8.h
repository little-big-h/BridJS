#ifndef _DYNLOAD_V8_H
#define _DYNLOAD_V8_H
#include <node.h>

namespace bridjs {
class Dynload {
public: 
static v8::Handle<v8::Value> loadLibrary(const v8::Arguments& args);
static v8::Handle<v8::Value> freeLibrary(const v8::Arguments& args);
static v8::Handle<v8::Value> findSymbol(const v8::Arguments& args);

static v8::Handle<v8::Value> symsInit(const v8::Arguments& args);
static v8::Handle<v8::Value> symsCleanup(const v8::Arguments& args);
static v8::Handle<v8::Value> symsCount(const v8::Arguments& args);
static v8::Handle<v8::Value> symsName(const v8::Arguments& args);
static v8::Handle<v8::Value> symsNameFromValue(const v8::Arguments& args);

}; // namespace dynload
}//
#endif  // _DYNLOAD_V8_H
