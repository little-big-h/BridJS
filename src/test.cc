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

		std::cout<<pTestStruct->e<<std::endl;

		return  pTestStruct->w* pTestStruct->x* pTestStruct->y* pTestStruct->z* pTestStruct->e;
	}


typedef struct
{
	char a, b, c;
} FewValues;

double sum_FewValues(FewValues values)
{
	printf("sum_FewValues(a = %d, b = %d, c = %d)\n", (int)values.a, (int)values.b, (int)values.c);
	return ((double)values.a) + ((double)values.b) + ((double)values.c);
}

void bridjs::Test::test(){
	DCCallVM* pc = dcNewCallVM(4096);
	{
		FewValues values;
		double calledSum, expectedSum;
		DCstruct* s = dcNewStruct(3, DEFAULT_ALIGNMENT);
		dcStructField(s, DC_SIGCHAR_CHAR, DEFAULT_ALIGNMENT, 1);
		dcStructField(s, DC_SIGCHAR_CHAR, DEFAULT_ALIGNMENT, 1);
		dcStructField(s, DC_SIGCHAR_CHAR, DEFAULT_ALIGNMENT, 1);
		dcCloseStruct(s);

		//DC_TEST_STRUCT_SIZE(FewValues, s);

		values.a = 1;
		values.b = 2;
		values.c = 3;

		dcMode(pc, DC_CALL_C_X64_WIN64);
		dcReset(pc);
		printf("BEFORE dcArgStruct\n");
		dcArgChar(pc, 1);
		dcArgChar(pc, 2);
		dcArgChar(pc, 3);
		//dcArgStruct(pc, s, &values);
		printf("AFTER dcArgStruct\n");
		calledSum = dcCallDouble(pc, (DCpointer)&sum_FewValues);
		expectedSum = sum_FewValues(values);

		//DC_TEST_INT_EQUAL(expectedSum, calledSum);
		dcFreeStruct(s);
	}
	

	dcFree(pc);

}

}