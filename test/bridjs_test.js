var assert = require('assert'), bridjs = require('../lib/bridjs.js'), log4js = require("log4js"),
        Utils = require("../lib/Utils.js"), my = require('myclass');
var log = log4js.getLogger("bridjs_test");
var libPath = process.cwd() + '/lib/bridjs/' + bridjs.platformTag + '/bridjs.node';

setInterval(function(){log.info("keep test alive");},999999999);
//require('http').createServer().listen(8080);
//var lib = bridjs.rawBindings.dl.loadLibrary(libPath);
{/*Test dynload block*/
    var lib = null;
    //log.info("Test dynload: "+bridjs.Pointer);
    //console.log(libPath); 
    try {
        var lib = bridjs.dl.loadLibrary(libPath);
        //var testMultiplyFunction= bridjs.dl.findSymbol(lib, "testMultiplyFunction");
        log.info("Lib: " + JSON.stringify(lib) + ", symbols: " + bridjs.symbols(libPath));
        /*
         assert(
         symbols.length > 10,
         "Didn't find expected symbols in " + libPath);*/
        log.info("Test dynload pass");
    } catch (e) {
        log.error(e);
    } finally {
        if (lib) {
            bridjs.dl.freeLibrary(lib);
        }
    }

}
{/*Test dyncall block*/
    log.info("Test dyncall");

    var lib = null, vm = null, testMultiplyFunction, dyncall = bridjs.dc, ret,
            Signature = bridjs.dc.Signature, NativeFunction = bridjs.dc.NativeFunction, nativeFunction,
            startSeconds, CallbackObject, afterDyncallTest, afterDyncallbackTest,callbackHandler, 
            i, iteration = 100000, dcbCallbackCount = 2;
    //console.log(libPath); 

    CallbackObject = my.Class({
        onDone: function(vm, result) {
            log.info("onDone: " + result);
            afterDyncallTest();
        },
        onError: function(vm, result) {
            log.info("onError: " + result);
        }
    });

    DcbCallbackObject = my.Class({
        onDone: function(w, x, y, z, e) {
            var value = w * x * y * z * e;
            
            --dcbCallbackCount;

            log.info("onDcbCallback: " + value+", count = "+dcbCallbackCount);
            
            if(dcbCallbackCount<=0){
                setTimeout(afterDyncallbackTest, 1000);
            }
            
            return value;
        }
    });

    try {

        startSeconds = Utils.timeSeconds();
        for (i = 0; i < iteration; ++i) {
            ret = bridjs.test.testMultiplyFunction(2, 2, 2, 2, 2.5);
        }
        log.info("Spend " + ((Utils.timeSeconds() - startSeconds) / iteration) + " to invoke testMultiplyFunction by native binding");
        log.info("2 x 2 x 2 x 2 x 2.5 = " + ret);

        lib = bridjs.dl.loadLibrary(libPath);
        testMultiplyFunction = bridjs.dl.findSymbol(lib, "testMultiplyFunction");
        vm = dyncall.newCallVM(4096);
        //log.info("vm pointer: "+JSON.stringify(vm));
        dyncall.mode(vm, bridjs.dc.CALL_C_DEFAULT);
        dyncall.reset(vm);
        startSeconds = Utils.timeSeconds();
        for (i = 0; i < iteration; ++i) {
            dyncall.reset(vm);
            dyncall.argShort(vm, 2);
            dyncall.argInt(vm, 2);
            dyncall.argLong(vm, 2);
            dyncall.argLongLong(vm, 2);
            dyncall.argDouble(vm, 2.5);
            ret = dyncall.callDouble(vm, testMultiplyFunction);
        }
        log.info("Spend " + ((Utils.timeSeconds() - startSeconds) / iteration) + " to invoke testMultiplyFunction by dyncall");
        assert(ret === 40, "Call testMultiplyFunction fail");


        assert(Signature.INT32_TYPE === 'i', "invalid int32 type");
        assert(Signature.INT_TYPE === 'i', "invalid int32 type");

        nativeFunction = new NativeFunction(testMultiplyFunction,
                Signature.DOUBLE_TYPE, Signature.INT16_TYPE, Signature.INT32_TYPE,
                Signature.LONG_TYPE, Signature.LONGLONG_TYPE, Signature.DOUBLE_TYPE);

        log.info("returnType = " + nativeFunction.getReturnType() + ", argumentsLength = " + nativeFunction.getArgumentsLength());

        startSeconds = Utils.timeSeconds();
        for (i = 0; i < iteration; ++i) {
            ret = nativeFunction.call(vm, 2, 2, 2, 2, 2.5);
        }
        log.info("Spend " + ((Utils.timeSeconds() - startSeconds) / iteration) + " to invoke testMultiplyFunction by NativeFunction");
        log.info("2 x 2 x 2 x 2 x 2.5 = " + nativeFunction.call(vm, 2, 2, 2, 2, 2.5));

        nativeFunction.callAsync(vm, 2, 2, 2, 2, 2.5, new CallbackObject());
        /*
         assert(signature.getReturnType()===1, "Get return type fail");
         assert(signature.getArgumentsLength()===6, "Get arguments length fail");
         assert(signature.getArgumentType(2)===3, "Get argument type fail");
         */
        log.info("Test dyncall pass");
    } catch (e) {
        log.error(e);
    } finally {
        /*
         if(vm){
         bridjs.dc.free(vm);
         }
         
         if(lib){
         bridjs.dl.freeLibrary(lib);
         }*/
    }

    afterDyncallTest = function() {
        var testCallbackFunction, testAsyncCallbackFunction;
        log.info("Test dyncallback start:");

        callbackHandler = bridjs.dcb.newCallback(Signature.DOUBLE_TYPE, [Signature.INT16_TYPE, Signature.INT32_TYPE,
            Signature.LONG_TYPE, Signature.LONGLONG_TYPE, Signature.DOUBLE_TYPE], new DcbCallbackObject());

        testCallbackFunction = bridjs.dl.findSymbol(lib, "testCallbackFunction");
        testAsyncCallbackFunction = bridjs.dl.findSymbol(lib, "testAsyncCallbackFunction");
        
        /*testCallbackFunction*/
        startSeconds = Utils.timeSeconds();
        //for(i=0;i<iteration;++i){
        dyncall.reset(vm);
        dyncall.argPointer(vm, callbackHandler);
        dyncall.callVoid(vm, testCallbackFunction);
        //}
        log.info("Spend " + ((Utils.timeSeconds() - startSeconds)) + " to invoke testCallbackFunction by dyncall");
        
        /*testAsyncCallbackFunction*/
        startSeconds = Utils.timeSeconds();
        //for(i=0;i<iteration;++i){
        dyncall.reset(vm);
        dyncall.argPointer(vm, callbackHandler);
        dyncall.callVoid(vm, testAsyncCallbackFunction);
        //}
        log.info("Spend " + ((Utils.timeSeconds() - startSeconds)) + " to invoke testAsyncCallbackFunction by dyncall");

        log.info("callback handler: " + testCallbackFunction);
    };
    afterDyncallbackTest = function() {
        if (callbackHandler) {
            bridjs.dcb.freeCallback(callbackHandler);
        }
        log.info("Test dyncallback pass");

        log.info("Release dyncall resources");
        if (vm) {
            bridjs.dc.free(vm);
        }

        if (lib) {
            bridjs.dl.freeLibrary(lib);
        }
    };

}