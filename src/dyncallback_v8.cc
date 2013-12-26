#include "dyncallback_v8.h"
#include "dyncall_v8_utils.h"

#include <map>
#include <iostream>
#include <memory>

using namespace v8;
using namespace bridjs;

std::map<DCCallback*, CallbackWrapper*> gValueWrapperMap;
uv_mutex_t gValueWrapperMutex;
unsigned long gDefaultThread;

char callbackHandler(DCCallback* cb, DCArgs* args, DCValue* result, void* userdata);

v8::Handle<v8::Value> NewCallback(const v8::Arguments& args);
v8::Handle<v8::Value> InitCallback(const v8::Arguments& args);
v8::Handle<v8::Value> FreeCallback(const v8::Arguments& args);


v8::Handle<v8::Value> ArgBool     (const v8::Arguments& args);
v8::Handle<v8::Value> ArgChar     (const v8::Arguments& args);
v8::Handle<v8::Value> ArgShort    (const v8::Arguments& args);
v8::Handle<v8::Value> ArgInt      (const v8::Arguments& args);
v8::Handle<v8::Value> ArgLong     (const v8::Arguments& args);
v8::Handle<v8::Value> ArgLongLong (const v8::Arguments& args);
v8::Handle<v8::Value> ArgUChar    (const v8::Arguments& args);
v8::Handle<v8::Value> ArgUShort   (const v8::Arguments& args);
v8::Handle<v8::Value> ArgUInt     (const v8::Arguments& args);
v8::Handle<v8::Value> ArgULong    (const v8::Arguments& args);
v8::Handle<v8::Value> ArgULongLong(const v8::Arguments& args);
v8::Handle<v8::Value> ArgFloat    (const v8::Arguments& args);
v8::Handle<v8::Value> ArgDouble   (const v8::Arguments& args);
v8::Handle<v8::Value> ArgPointer  (const v8::Arguments& args);


void bridjs::Dyncallback::Init(v8::Handle<v8::Object> exports){
	int32_t error;

	gDefaultThread = uv_thread_self();
	memset(&gValueWrapperMutex,0, sizeof(uv_mutex_t));
	error = uv_mutex_init(&gValueWrapperMutex);

	if(error!=0){
		char* message = "Fail to init gLoadLubraryMutex";
		std::cerr<<message<<std::endl;

		throw std::runtime_error(message);
	}


	NODE_SET_METHOD(exports,"newCallback", NewCallback);
	NODE_SET_METHOD(exports,"initCallback", InitCallback);
	NODE_SET_METHOD(exports,"freeCallback", FreeCallback);

	NODE_SET_METHOD(exports,"argBool", ArgBool);
	NODE_SET_METHOD(exports,"argChar", ArgChar);
	NODE_SET_METHOD(exports,"argShort", ArgShort);
	NODE_SET_METHOD(exports,"argInt", ArgInt);
	NODE_SET_METHOD(exports,"argLong", ArgLong);
	NODE_SET_METHOD(exports,"argLongLong", ArgLongLong);
	NODE_SET_METHOD(exports,"argUChar", ArgUChar);
	NODE_SET_METHOD(exports,"argUShort", ArgUShort);
	NODE_SET_METHOD(exports,"argUint", ArgUInt);
	NODE_SET_METHOD(exports,"argULong", ArgULong);
	NODE_SET_METHOD(exports,"argULongLong", ArgULongLong);
	NODE_SET_METHOD(exports,"argFloat", ArgFloat);
	NODE_SET_METHOD(exports,"argPointer", ArgPointer);
}

v8::Handle<v8::Value> NewCallback(const v8::Arguments& args){
	 HandleScope scope;
	 std::stringstream signature;
	 std::vector<const char> argumentTypes;
	 GET_CHAR_ARG(returnType,args,0);
	 GET_ARRAY_ARG(argumentArray,args,1);
		 GET_OBJECT_ARG(callbackObj,args,2);
		 CallbackWrapper* valueWrapper;
		 DCCallback* pCallback;

		 for(uint32_t i =0;i<argumentArray->Length();++i){
			 GET_CHAR_VALUE(type, argumentArray->Get(i),i);
			 argumentTypes.push_back(type);
			 signature<<type;
		 }
		 signature<<')'<<returnType;

		 valueWrapper = new CallbackWrapper(returnType,argumentTypes, v8::Persistent<Object>::New(callbackObj));//GET_POINTER_ARG(void,pUserData,args,2);
		 pCallback =dcbNewCallback(signature.str().c_str(),callbackHandler,valueWrapper);
	 
		 if(pCallback!=NULL){
			 uv_mutex_lock(&gValueWrapperMutex);
			 gValueWrapperMap[pCallback] = valueWrapper;
			 uv_mutex_unlock(&gValueWrapperMutex);
			 return scope.Close(bridjs::Utils::wrapPointer(pCallback));
		 }else{
			 delete valueWrapper;
			 valueWrapper = NULL;
			 return THROW_EXCEPTION("Fail to new Callback object");
		 }
}
v8::Handle<v8::Value> InitCallback(const v8::Arguments& args){
	HandleScope scope;
	std::stringstream signature;
	std::vector<const char> argumentTypes;
	 GET_POINTER_ARG(DCCallback,pCallback,args,0);
	 GET_ARRAY_ARG(argumentArray,args,1);
	 GET_CHAR_ARG(returnType,args,2);
	 GET_OBJECT_ARG(callbackObj,args,3);

		 for(uint32_t i =0;i<argumentArray->Length();++i){
			 GET_CHAR_VALUE(type, argumentArray->Get(i),i);
			 argumentTypes.push_back(type);
			 signature<<type;
		 }
		 signature<<')'<<returnType;
	
	 if(pCallback!=NULL){
		  CallbackWrapper* valueWrapper = new CallbackWrapper(returnType,argumentTypes, v8::Persistent<Object>::New(callbackObj));

		 dcbInitCallback(pCallback,signature.str().c_str(),callbackHandler,valueWrapper);

		 uv_mutex_lock(&gValueWrapperMutex);
		 gValueWrapperMap[pCallback] = valueWrapper;
		 uv_mutex_unlock(&gValueWrapperMutex);

		 return scope.Close(v8::Undefined());
	 }else{
		 return THROW_EXCEPTION("pCallback was NULL");
	 }
}
v8::Handle<v8::Value> FreeCallback(const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCCallback,pCallback,args,0);

	if(pCallback!=NULL){
		uv_mutex_lock(&gValueWrapperMutex);

		std::map<DCCallback*, CallbackWrapper*>::iterator iterator = gValueWrapperMap.find(pCallback);

		if(iterator!=gValueWrapperMap.end()){
			CallbackWrapper* valueWrapper = iterator->second;

			if( valueWrapper!=NULL){
				delete  valueWrapper;
				valueWrapper = NULL;
			}
		}
		 uv_mutex_unlock(&gValueWrapperMutex);

		dcbFreeCallback(pCallback);
		return scope.Close(v8::Undefined());
	}else{
		return THROW_EXCEPTION("pCallback was NULL");
	}
}

v8::Handle<v8::Value> ArgBool     (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_BOOL(dcbArgBool(pArgs)));
}
v8::Handle<v8::Value> ArgChar     (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(bridjs::Utils::toV8String(dcbArgChar(pArgs)));
}
v8::Handle<v8::Value> ArgShort    (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_SHORT(dcbArgShort(pArgs)));
}
v8::Handle<v8::Value> ArgInt      (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_INT(dcbArgInt(pArgs)));
}
v8::Handle<v8::Value> ArgLong     (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_LONG(dcbArgLong(pArgs)));
}
v8::Handle<v8::Value> ArgLongLong (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_LONGLONG(dcbArgLongLong(pArgs)));
}
v8::Handle<v8::Value> ArgUChar    (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(bridjs::Utils::toV8String(dcbArgUChar(pArgs)));
}
v8::Handle<v8::Value> ArgUShort   (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_USHORT(dcbArgUShort(pArgs)));
}
v8::Handle<v8::Value> ArgUInt     (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_UINT(dcbArgUInt(pArgs)));
}
v8::Handle<v8::Value> ArgULong    (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_ULONG(dcbArgULong(pArgs)));
}
v8::Handle<v8::Value> ArgULongLong(const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_ULONGLONG(dcbArgULongLong(pArgs)));
}
v8::Handle<v8::Value> ArgFloat    (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_FLOAT(dcbArgFloat(pArgs)));
}
v8::Handle<v8::Value> ArgDouble   (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_DOUBLE(dcbArgDouble(pArgs)));
}
v8::Handle<v8::Value> ArgPointer  (const v8::Arguments& args){
	HandleScope scope;
	GET_POINTER_ARG(DCArgs,pArgs,args,0);

	return scope.Close(WRAP_POINTER(dcbArgPointer(pArgs)));
}

std::shared_ptr<v8::Handle<Value>> pullArgs(DCCallback* cb, DCArgs* args, CallbackWrapper* pCallbackWrapper){
	//HandleScope scope;
	const size_t length = pCallbackWrapper->getArgumentLength();
	std::shared_ptr<v8::Handle<Value>> argv(new v8::Handle<Value>[length], ArrayDeleter<v8::Handle<Value>>());

	for(uint32_t i=0; i<length;++i){

		switch(pCallbackWrapper->getArgumentType(i)){
		case DC_SIGCHAR_BOOL:{
			argv.get()[i] = WRAP_BOOL(dcbArgBool(args));
			}
			break;
		case DC_SIGCHAR_UCHAR:{
			argv.get()[i] =WRAP_UCHAR(dcbArgUChar(args));
			}
			break;
		case DC_SIGCHAR_CHAR:{
			argv.get()[i] =WRAP_CHAR(dcbArgChar(args));
			}
			break;
        case DC_SIGCHAR_SHORT:{
			argv.get()[i] =WRAP_SHORT(dcbArgShort(args));
			}
			break;
		case DC_SIGCHAR_USHORT:{
			argv.get()[i] =WRAP_USHORT(dcbArgUShort(args));
			}
			break;
        case DC_SIGCHAR_INT:{
			argv.get()[i] =WRAP_INT(dcbArgInt(args));
			}
			break;
		case DC_SIGCHAR_UINT:{
			argv.get()[i] =WRAP_UINT(dcbArgUInt(args));
			}
			break;
        case DC_SIGCHAR_LONG:{
			argv.get()[i] =WRAP_LONG(dcbArgLong(args));
			}
			break;
		case DC_SIGCHAR_ULONG:{
			argv.get()[i] =WRAP_ULONG(dcbArgULong(args));
			}
			break;
        case DC_SIGCHAR_LONGLONG:{
			argv.get()[i] =WRAP_LONGLONG(dcbArgLongLong(args));
			}
			break;
		case DC_SIGCHAR_ULONGLONG:{
			argv.get()[i] =WRAP_ULONGLONG(dcbArgULongLong(args));
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			argv.get()[i] =WRAP_FLOAT(dcbArgFloat(args));
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			argv.get()[i] =WRAP_DOUBLE(dcbArgDouble(args));
			}
			break; 
		case DC_SIGCHAR_STRING:{
			argv.get()[i] =WRAP_STRING((const char*)dcbArgPointer(args));
			}
			break;
        case DC_SIGCHAR_POINTER:{
			argv.get()[i] =WRAP_POINTER(dcbArgPointer(args));
			}
			break;  
		case DC_SIGCHAR_STRUCT:{
			std::cerr<<"Not implement"<<std::endl;
			argv.get()[i] =WRAP_POINTER(dcbArgPointer(args));
			//return v8::Exception::TypeError(v8::String::New("Not implement"));
		    }
			break;
		}
    }

	return argv;
}

v8::Handle<Value> setReturnValue(DCValue* value,Local<Value> returnValue, CallbackWrapper* pCallbackWrapper){
	//DCCallVM *vm = nativeFunction->getVM();
	HandleScope scope;

	switch(pCallbackWrapper->getReturnType()){
		case DC_SIGCHAR_VOID:{
			//dothing
			}
			break;
		case DC_SIGCHAR_BOOL:{
			GET_BOOL_VALUE(rValue, returnValue,0);
			value->B = rValue;
			}
			break;
		case DC_SIGCHAR_UCHAR:{
			GET_CHAR_VALUE(rValue, returnValue,0);
			value->C = rValue;
			}
			break;
		case DC_SIGCHAR_CHAR:{
			GET_CHAR_VALUE(rValue, returnValue,0);
			value->c = rValue;
			}
			break;
        case DC_SIGCHAR_SHORT:{
			GET_INT32_VALUE(rValue, returnValue,0);
			value->s = static_cast<DCushort>(rValue);
			}
			break;
		case DC_SIGCHAR_USHORT:{
			GET_UINT32_VALUE(rValue, returnValue,0);
			value->S = static_cast<DCshort>(rValue);
			}
			break;
        case DC_SIGCHAR_INT:{
			GET_INT32_VALUE(rValue, returnValue,0);
			value->i = (rValue);
			}
			break;
		case DC_SIGCHAR_UINT:{
			GET_UINT32_VALUE(rValue, returnValue,0);
			value->I = (rValue);
			}
			break; 
        case DC_SIGCHAR_LONG:{
			GET_INT64_VALUE(rValue, returnValue,0);
			value->j = static_cast<DClong>(rValue);
			}
			break;
		case DC_SIGCHAR_ULONG:{
			GET_INT64_VALUE(rValue, returnValue,0);
			value->J = static_cast<DCulong>(rValue);
			}
			break;
		case DC_SIGCHAR_LONGLONG:{
			GET_INT64_VALUE(rValue, returnValue,0);
			value->l = static_cast<DClonglong>(rValue);
			}
			break;
		case DC_SIGCHAR_ULONGLONG:{
			GET_INT64_VALUE(rValue, returnValue,0);
			value->L = static_cast<DClonglong>(rValue);
			}
			break;
        case DC_SIGCHAR_FLOAT:{
			GET_FLOAT_VALUE(rValue, returnValue,0);
			value->f = (rValue);
			}
			break;
        case DC_SIGCHAR_DOUBLE:{
			GET_DOUBLE_VALUE(rValue, returnValue,0);
			value->d = (rValue);
			}
			break;
		case DC_SIGCHAR_STRING:{
			GET_POINTER_VALUE(const char,rValue,returnValue,0);
			value->Z = rValue;
			}
		    break;
        case DC_SIGCHAR_POINTER:{
			GET_POINTER_VALUE(void, rValue,returnValue,0);
			value->p = (rValue);
			}
			break;
		case DC_SIGCHAR_STRUCT:{
			std::cerr<<"Not implement"<<std::endl;
			GET_POINTER_VALUE(void, rValue,returnValue,0);
			value->p = (rValue);
		    }
			break;

		default:
			std::stringstream message;
			message<<"Unknown returnType: "<<pCallbackWrapper->getReturnType()<<std::endl;
			//throw std::runtime_error(message.str());
			return v8::Exception::TypeError(v8::String::New(message.str().c_str()));
		}

	return scope.Close(v8::Null());
}

char callbackHandler(DCCallback* cb, DCArgs* args, DCValue* result, void* userdata){
	CallbackWrapper *pCallback = static_cast<CallbackWrapper*>(userdata);

	if(pCallback!=NULL){
		pCallback->onCallback(cb,args,result);

		return pCallback->getReturnType();
	}else{
		std::cerr<<"Fail to cast data pointer to CallbackWrapper"<<std::endl;

		return DC_SIGCHAR_VOID;
	}
}

void invokeV8Callback(uv_async_t *handle, int status /*UNUSED*/) {
	CallbackTask *pCallTask = static_cast<CallbackTask*>(handle->data);

	if(pCallTask!=NULL){
		pCallTask->done();
	}else{
		std::cerr<<"Fail to cast data pointer to CallbackTask"<<std::endl;
	}
}

void closeAsyncCallback(uv_handle_t *handle) {
	CallbackTask *pCallTask = static_cast<CallbackTask*>(handle->data);
	
	if(pCallTask!=NULL){
		//std::cout<<pCallTask<<std::endl;
		delete pCallTask;
	}else{
		std::cerr<<"Fail to cast data pointer to CallbackTask"<<std::endl;
	}
}

bridjs::CallbackWrapper::CallbackWrapper(const char returnType, const std::vector<const char> &argumentTypes, v8::Persistent<v8::Object> pCallbackObject):
	NativeFunction(NULL, returnType, argumentTypes),mpCallbackObject(pCallbackObject) {
	int32_t error;	
	memset(&mMutex,0, sizeof(uv_mutex_t));
    error = uv_mutex_init(&mMutex);

	if(error!=0){
		std::ostringstream message;

		message<<"bridjs::CallbackWrapper::CallbackWrapper() => Fail to init mMutex, error = "<<error;
		std::cerr<<message<<std::endl;

		throw std::runtime_error(message.str());
	}

}

const char bridjs::CallbackWrapper::onCallback(DCCallback* cb, DCArgs* args, DCValue* result){
	try{
		uv_mutex_lock(&mMutex);

		CallbackTask *task = new CallbackTask(this,cb,args,result);
		uv_work_t *req = new uv_work_t;

		req->data = task;

		
		if(uv_thread_self()==gDefaultThread){
			invokeV8Callback(task->getAsync(),0);
		}else{
			//uv_queue_work(uv_default_loop(),req,NULL,(uv_after_work_cb)invokeV8Callback);
			uv_async_send(task->getAsync());
			//std::cout<<"1111111111111"<<std::endl;	
			task->wait();
			//std::cout<<"2222222222222"<<std::endl;	
		}
		

		req->data = NULL;
		delete req;
		/*Delete pTask object after close async object*/
		uv_close((uv_handle_t*) task->getAsync(),closeAsyncCallback);
		req = NULL;
		task = NULL;
	}catch(...){
		std::cerr<<"bridjs::CallbackWrapper::onCallback=>Unknown exception"<<std::endl;
	}

	uv_mutex_unlock(&mMutex);

	return this->mReturnType;
}

uv_mutex_t* CallbackWrapper::getMutex(){
	return &this->mMutex;
}

bridjs::CallbackWrapper::~CallbackWrapper(){
	uv_mutex_destroy(&mMutex);
	this->mpCallbackObject.Dispose();
	this->mpCallbackObject.Clear();
}

CallbackTask::CallbackTask(CallbackWrapper *pCallbackWrapper, DCCallback *pDCCallBack, DCArgs *pDCArgs, DCValue* pDCresult):mpCallbackWrapper(pCallbackWrapper),
	mpDCCallBack(pDCCallBack), mpDCArgs(pDCArgs), mpDCresult(pDCresult){
	int32_t error;

	memset(&mCond,0, sizeof(uv_cond_t));
	memset(&mAsync,0,sizeof(uv_async_t));

	uv_async_init(uv_default_loop(),&mAsync,invokeV8Callback);

	mAsync.data = this;

    error = uv_cond_init(&mCond);

	if(error!=0){
		std::ostringstream message;

		message<<"CallbackTask::CallbackTask() => Fail to init mMutex, error = "<<error;
		std::cerr<<message<<std::endl;

		throw std::runtime_error(message.str());
	}	
}

uv_async_t* CallbackTask::getAsync(){
	return &mAsync;
}

void CallbackTask::wait(){
	uv_cond_wait(&mCond, this->mpCallbackWrapper->getMutex());
}

void CallbackTask::notify(){
	uv_cond_signal(&mCond);
}

void CallbackTask::done(){
	HandleScope scope;
	std::shared_ptr<v8::Handle<Value>> argv = pullArgs(this->mpDCCallBack, this->mpDCArgs,this->mpCallbackWrapper);
    v8::Local<Value> onDoneValue = this->mpCallbackWrapper->mpCallbackObject->GetRealNamedProperty(v8::String::New("onDone"));

		if(onDoneValue->IsFunction()){
			v8::Local<v8::Function> onDoneFunction = v8::Local<Function>::Cast(onDoneValue);
			v8::Local<v8::Value> returnValue = onDoneFunction->Call(this->mpCallbackWrapper->mpCallbackObject,
				static_cast<int32_t>(this->mpCallbackWrapper->getArgumentLength()), 
				argv.get());

			setReturnValue(this->mpDCresult,returnValue,this->mpCallbackWrapper);
		}else{
			std::cerr<<"Illegal callback object: "<<(*v8::String::Utf8Value(this->mpCallbackWrapper->mpCallbackObject->ToString()))<<std::endl;
		}
		//std::cout<<"43333333"<<std::endl;	
	this->notify();
}

CallbackTask::~CallbackTask(){
	uv_cond_destroy(&mCond);
	//std::cout<<this<<std::endl;
	
}
