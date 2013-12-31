var assert = require('assert'), bridjs = require('../lib/bridjs.js'), log4js = require("log4js"),
        Utils = require("../lib/utils.js"), my = require('myclass');
var log = log4js.getLogger("BridjsTest"), libPath;

var interval = setInterval(function() {
    log.info("keep test alive");
}, 999999999);
var lib;

{/*Test dynload block*/
    libPath = __dirname + "/" + bridjs.LIBRARY_PATH + ".node";
    lib = bridjs.dl.loadLibrary("libPath");

    log.info("Lib: " + JSON.stringify(lib) + ", symbols: " + bridjs.symbols(libPath));
    log.info("Test dynload pass");

}
{/*Test dyncall block*/
    log.info("Test dyncall");

    var lib = null, vm = null, testMultiplyFunction, dyncall = bridjs.dc, ret,
            Signature = bridjs.dc.Signature, NativeFunction = bridjs.dc.NativeFunction, nativeFunction,
            startSeconds, CallbackObject, afterDyncallTest, afterDyncallbackTest, callbackHandler,
            i, iteration = 100000, dcbCallbackCount = 2, testStruct, testStructBuffer,
            offset, os = require("os"), typeSize, testStructFunction, structObject, testStructValueFunction;
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

            log.info("onDcbCallback: " + value + ", count = " + dcbCallbackCount);

            if (dcbCallbackCount <= 0) {
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
        testStructFunction = bridjs.dl.findSymbol(lib, "testStructFunction");
        testStructValueFunction = bridjs.dl.findSymbol(lib, "testStructValueFunction");

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

        if (testStructFunction) {
            structObject = new bridjs.dyncall.Struct(Signature.INT16_TYPE, Signature.INT32_TYPE,
                    Signature.LONG_TYPE, Signature.LONGLONG_TYPE, Signature.DOUBLE_TYPE);
            testStruct = dyncall.newStruct(5, dyncall.DEFAULT_ALIGNMENT);
            dyncall.structField(testStruct, Signature.INT16_TYPE, dyncall.DEFAULT_ALIGNMENT, 1);
            dyncall.structField(testStruct, Signature.INT32_TYPE, dyncall.DEFAULT_ALIGNMENT, 1);
            dyncall.structField(testStruct, Signature.LONG_TYPE, dyncall.DEFAULT_ALIGNMENT, 1);
            dyncall.structField(testStruct, Signature.LONGLONG_TYPE, dyncall.DEFAULT_ALIGNMENT, 1);
            dyncall.structField(testStruct, Signature.DOUBLE_TYPE, dyncall.DEFAULT_ALIGNMENT, 1);
            dyncall.closeStruct(testStruct);


            testStructBuffer = new Buffer(structObject.getSize());

            structObject.setField(0, 2, testStructBuffer);
            structObject.setField(1, 2, testStructBuffer);
            structObject.setField(2, 2, testStructBuffer);
            structObject.setField(3, 2, testStructBuffer);
            structObject.setField(4, 2.5, testStructBuffer);

            assert(structObject.getField(3, testStructBuffer) === 2, "Call Struct.getField fail");
            startSeconds = Utils.timeSeconds();
            //for (i = 0; i < iteration; ++i) {
            dyncall.reset(vm);
            dyncall.argPointer(vm, testStructBuffer);
            ret = dyncall.callDouble(vm, testStructFunction);
            log.info("testStructFunction: " + ret);
            log.info("Spend " + ((Utils.timeSeconds() - startSeconds)) + " to invoke testStructFunction by dyncall");
            assert(ret === 40, "Call testStructFunction fail");

            dyncall.freeStruct(testStruct);
            testStruct = null;
        } else {
            throw "Fail to locate testStructFunction from native lirary";
        }
       
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

        /*Test Prototype binding*/
        {
            log.info("Test prototype binding start");

            var Tester = my.Class({
                testMultiplyFunction: bridjs.defineFunction(Signature.DOUBLE_TYPE, Signature.INT16_TYPE, Signature.INT32_TYPE,
                        Signature.LONG_TYPE, Signature.LONGLONG_TYPE, Signature.DOUBLE_TYPE),
                testStructFunction: bridjs.defineFunction(Signature.DOUBLE_TYPE, Signature.POINTER_TYPE),
                testComplexStructFunction :  bridjs.defineFunction(Signature.DOUBLE_TYPE, Signature.POINTER_TYPE),
                testArrayStructFunction : bridjs.defineFunction(Signature.DOUBLE_TYPE, Signature.POINTER_TYPE),
                testAsyncCallbackFunction : bridjs.defineFunction(Signature.VOID_TYPE, Signature.POINTER_TYPE)
            }), testerInstance, TestStruct, testStruct, TestComplexStruct, Point2d,Point3d, 
                testComplexStruct, point3d, TestArrayStruct, testArrayStruct, callback;

            bridjs.register(Tester, libPath);



            testerInstance = new Tester();
            //log.info("Register Tester.testMultiplyFunctio: "+testerInstance.testMultiplyFunction);
            startSeconds = Utils.timeSeconds();
            for (i = 0; i < iteration; ++i) {
                ret = testerInstance.testMultiplyFunction(2, 2, 2, 2, 2.5);
            }
            log.info("Spend " + ((Utils.timeSeconds() - startSeconds) / iteration) + " to invoke Tester.testStructFunction by prototype binding");
            assert(ret === 40, "Call Tester.testMultiplyFunction fail");


            bridjs.async(testerInstance).testMultiplyFunction(2, 2, 2, 2, 2.5, function(result) {
                log.info(" bridjs.async(testerInstance).testMultiplyFunction results: " + result);
            });
            
            callback = bridjs.newCallback(bridjs.defineFunction(Signature.DOUBLE_TYPE, Signature.INT16_TYPE, Signature.INT32_TYPE,
            Signature.LONG_TYPE, Signature.LONGLONG_TYPE, Signature.DOUBLE_TYPE), function(w, x, y, z, e) {
                return w * x * y * z * e;
            });
            testerInstance.testAsyncCallbackFunction(callback);
            
            //log.info(bridjs.async(testerInstance));
 
            TestStruct = bridjs.defineStruct({
                x : bridjs.structField(Signature.INT16_TYPE,0),
                y : bridjs.structField(Signature.INT32_TYPE,1),
                z : bridjs.structField(Signature.LONG_TYPE,2),
                w : bridjs.structField(Signature.LONGLONG_TYPE,3),
                e : bridjs.structField(Signature.DOUBLE_TYPE,4)
            });
            Point2d = my.Class(bridjs.Struct,{
                constructor:function(){
                    Point2d.Super.call(this);
                },
                x : bridjs.structField(Signature.DOUBLE_TYPE,0),
                y : bridjs.structField(Signature.DOUBLE_TYPE,1)
            });
            Point3d = my.Class(bridjs.Struct,{
                constructor:function(){
                    Point3d.Super.call(this);
                },
                x : bridjs.structField(Signature.DOUBLE_TYPE,0),
                y : bridjs.structField(Signature.DOUBLE_TYPE,1),
                z : bridjs.structField(Signature.DOUBLE_TYPE,2)
            });
            //log.info(Signature.INT16_TYPE);
            TestComplexStruct = my.Class(bridjs.Struct,{
                constructor:function(){
                    TestComplexStruct.Super.call(this);
                },
                w:bridjs.structField(Signature.CHAR_TYPE,0),
		subStruct:bridjs.structField(TestStruct,1),
		x:bridjs.structField(Signature.INT16_TYPE,2),
		point2d:bridjs.structField(Point2d,3),
		y:bridjs.structField(Signature.INT32_TYPE,4),
		point3d:bridjs.structField(Point3d,5),
		z:bridjs.structField(Signature.INT64_TYPE,6)
            });
            
            TestArrayStruct = my.Class(bridjs.Struct,{
                constructor:function(){
                    TestArrayStruct.Super.call(this);
                },
                w:bridjs.structField(Signature.CHAR_TYPE,0),
                first:bridjs.structArrayField(Signature.CHAR_TYPE,3,1),
                second:bridjs.structArrayField(Signature.CHAR_TYPE,3,2)
            });
            testStruct = new TestStruct();
            testStruct.x = testStruct.y = testStruct.z = testStruct.w = 2;
            testStruct.e = 2.5;
            assert(testStruct.x === 2, "Struct's get/set property fail");
            //log.info(testStruct.getPointer());
            startSeconds = Utils.timeSeconds();
            for (i = 0; i < iteration; ++i) {
                ret = testerInstance.testStructFunction(bridjs.getStructPointer(testStruct));
            }
            log.info("Spend " + ((Utils.timeSeconds() - startSeconds) / iteration) + " to invoke Tester.testStructFunction by prototype binding");
            assert(ret === 40, "Call Tester.testStructFunction fail");
            
            testComplexStruct = new TestComplexStruct();
            ret = testComplexStruct.subStruct;
            assert((ret instanceof bridjs.Struct), "Fail to get sub-struct");
            
            testComplexStruct.w = testComplexStruct.x = testComplexStruct.y = testComplexStruct.z = 2;
            testComplexStruct.subStruct.e =  testComplexStruct.point2d.x = testComplexStruct.point3d.y = 2.5;
            ret = testComplexStruct.point3d.y;
            assert((ret === 2.5), "Fail to access sub-struct's element");
            
            ret = testerInstance.testComplexStructFunction(bridjs.getStructPointer(testComplexStruct));
            assert((ret === 250), "Fail to call testerInstance.testComplexStructFunction");
            
            point3d = new Point3d();
            point3d.x = point3d.y = point3d.z = 3.5;
            testComplexStruct.point3d = point3d;
            
            ret = testComplexStruct.point3d.z;
            assert((ret ===3.5), "Fail to set sub-struct");
            
            testArrayStruct = new TestArrayStruct();
            testArrayStruct.w = 1;
            testArrayStruct.first[1] = 2;
            testArrayStruct.second[2] = 's';
            
            ret = testArrayStruct.second[2];
            assert((ret ==='s'), "Fail to access array field");
            
            ret = testerInstance.testArrayStructFunction(bridjs.getStructPointer(testArrayStruct));
            assert((ret ===230), "Fail to access array field");
            try{
                ret = testerInstance.testArrayStructFunction(testArrayStruct);
                assert(false, "Fail to handle non-pointer type value");
            }catch(e){
                //pass
            }
            
            //bridjs.unregister(Tester);
            log.info("Test prototype binding pass");
        }
        //clearInterval(interval);
    };

}