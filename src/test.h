#pragma once

#include "dyncall_v8_utils.h"

#include <node.h>
#include <iostream>

extern "C"
{
	#include "dyncall.h"	
	#include "dyncall_signature.h"
}

typedef double (*MultiplyCallbackFunction)(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e);

using namespace v8;

namespace bridjs{
	class Test{
	public:
		static void asyncTestCallback(uv_work_t *req);
		static void afterCallAsync(uv_work_t *req);
		static Handle<v8::Value> TestMultiplyFunction(const v8::Arguments& args);
	};
}

extern "C"{
	UV_EXTERN double testMultiplyFunction(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e);
	UV_EXTERN void testCallbackFunction(MultiplyCallbackFunction callbackFunction);
	UV_EXTERN void testAsyncCallbackFunction(MultiplyCallbackFunction callbackFunction);
}
