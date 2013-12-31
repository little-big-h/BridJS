#include "test.h"
#include "dyncall.h"


using namespace v8;



void bridjs::Test::asyncTestCallback(uv_work_t *req){
	MultiplyCallbackFunction func = static_cast<MultiplyCallbackFunction>(req->data);
	//std::cout<<"Got it"<<std::endl;
	if(func !=NULL){
		std::cout<<"testCallbackFunction: "<<func(2,2,2,2,2.5)<<std::endl;
	}else{
		std::cerr<<"Fail to cast data pointer to MultiplyCallbackFunction"<<std::endl;
	}

	//std::cout<<"Got it"<<std::endl;
}

void bridjs::Test::afterCallAsync(uv_work_t *req){
	delete req;
	//std::cout<<"Got it2"<<std::endl;
}

Handle<v8::Value> bridjs::Test::TestMultiplyFunction(const v8::Arguments& args){
	HandleScope scope;

	GET_INT32_ARG(w,args,0);
	GET_INT32_ARG(x,args,1);
	GET_INT64_ARG(y,args,2);
	GET_INT64_ARG(z,args,3);
	GET_DOUBLE_ARG(e,args,4);

	return scope.Close(v8::Number::New(testMultiplyFunction(w,x,static_cast<const long>(y),z,e)));
}

extern "C"{
	double testMultiplyFunction(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e){
		return w*x*y*z*e;
	}

	void testCallbackFunction(MultiplyCallbackFunction callbackFunction){
		std::cout<<"testCallbackFunction: "<<callbackFunction(2,2,2,2,2.5)<<std::endl;
	}

	void testAsyncCallbackFunction(MultiplyCallbackFunction callbackFunction){
		uv_work_t *req = new uv_work_t();
		req->data = callbackFunction;

		uv_queue_work(uv_default_loop(),req,bridjs::Test::asyncTestCallback,(uv_after_work_cb)bridjs::Test::afterCallAsync);
	}

	double testStructFunction(const TestStruct *pTestStruct){

		//std::cout<<pTestStruct->e<<std::endl;

		return  pTestStruct->w* pTestStruct->x* pTestStruct->y* pTestStruct->z* pTestStruct->e;
	}

	double testStructValueFunction(const TestStruct testStruct){

		//std::cout<<pTestStruct->e<<std::endl;

		return  testStruct.w* testStruct.x* testStruct.y* testStruct.z* testStruct.e;
	}

	double testComplexStructFunction(const TestComplexStruct* pTestStruct){
		return  pTestStruct->w*pTestStruct->x*pTestStruct->y*pTestStruct->z*pTestStruct->point2d.x*pTestStruct->point3d.y*pTestStruct->subStruct.e;
	}

	double testArrayStructFunction(const TestArrayStruct* pTestStruct){
		return pTestStruct->w*pTestStruct->first[1]*pTestStruct->second[2];
	}
}