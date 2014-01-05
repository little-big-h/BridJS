#pragma once
#include "native_function_v8.h"

extern "C" {
#include "dyncall_callback.h"
}
namespace bridjs {

    class Dyncallback {
    public:
        static void Init(v8::Handle<v8::Object> exports);
    };

    class CallbackWrapper : public bridjs::NativeFunction {
        friend class CallbackTask;
    private:
        v8::Persistent<v8::Object> mpCallbackObject;
        uv_mutex_t mMutex;
    public:
        CallbackWrapper(const char returnType, const std::vector<char> &argumentTypes, v8::Persistent<v8::Object> pCallbackObject);
        const char onCallback(DCCallback* cb, DCArgs* args, DCValue* result);
        uv_mutex_t* getMutex();
        virtual ~CallbackWrapper();
    };

    class CallbackTask {
    private:
        bridjs::CallbackWrapper* mpCallbackWrapper;
        DCCallback* mpDCCallBack;
        DCArgs* mpDCArgs;
        DCValue* mpDCresult;
        uv_cond_t mCond;
        //uv_async_t mAsync;
    public:
        CallbackTask(CallbackWrapper *pCallbackWrapper, DCCallback *pDCCallBack, DCArgs *pDCArgs, DCValue* pDCresult);
        void wait();
        void notify();
        void done();
        //uv_async_t* getAsync();

        static void flushV8Callbacks(uv_async_t *handle, int status /*UNUSED*/);

        virtual ~CallbackTask();
    };

} // namespace dyncallback

