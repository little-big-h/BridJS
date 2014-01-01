#pragma once

#include "dyncall_v8_utils.h"

#include <node.h>
#include <iostream>

extern "C"
{
	#include "dyncall.h"	
	#include "dyncall_signature.h"
}

using namespace v8;

namespace bridjs{
	class Test{
	public:
		static void test();
		static void asyncTestCallback(uv_work_t *req);
		static void afterCallAsync(uv_work_t *req);
		static Handle<v8::Value> TestMultiplyFunction(const v8::Arguments& args);
	};
}

extern "C"{
	typedef struct{
		int16_t w;
		int32_t x;
		long y;
		LONGLONG z;
		double e;
	} TestStruct;

	typedef struct{
		double x;
		double y;
	} Point2d;

	typedef struct{
		double x;
		double y;
		double z;
	} Point3d;

	typedef struct{
		int8_t x;
		int8_t y;
		int8_t z;
	} Point3c;

	typedef struct{
		int8_t x;
		int8_t y;
		int8_t x1;
		int8_t y1;
	} Point2c;

	typedef struct{
		char w;
		char first[3];
		char second[3];
	} TestArrayStruct;

	typedef struct{
		int8_t w;
		TestStruct subStruct;
		int16_t x;
		Point2d point2d;
		int32_t y;
	} TempStruct2;

	typedef struct{
		char w;
		TestStruct subStruct;
		int16_t x;
		Point2d point2d;
		int32_t y;
		Point3d point3d;
		int64_t z;
	} TestComplexStruct;

	typedef double (*MultiplyCallbackFunction)(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e);
	typedef double (*TestStructCallbackFunction)(const TestStruct* pTestStruct);


	UV_EXTERN double testMultiplyFunction(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e);
	UV_EXTERN double testStructFunction(const TestStruct* pTestStruct);
	UV_EXTERN void testCallbackFunction(MultiplyCallbackFunction callbackFunction);
	UV_EXTERN void testAsyncCallbackFunction(MultiplyCallbackFunction callbackFunction);
	UV_EXTERN double testStructValueFunction(const TestStruct testStruct);
	UV_EXTERN double testComplexStructFunction(const TestComplexStruct* pTestStruct);
	UV_EXTERN double testArrayStructFunction(const TestArrayStruct* pTestStruct);
	UV_EXTERN const TestStruct* testStructPassByPointer(const TestStruct* pTestStruct);
	UV_EXTERN void testStructCallbackFunction(const TestStruct* pTestStruct,TestStructCallbackFunction callbackFunction);
}
