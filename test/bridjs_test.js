var assert = require('assert'), bridjs = require('../lib/bridjs.js'), log4js = require("log4js"),
        Utils = require("../lib/Utils.js");

var log = log4js.getLogger("bridjs_test");

var libPath = process.cwd() + '/lib/bridjs/' + bridjs.platformTag + '/bridjs.node';
//var lib = bridjs.rawBindings.dl.loadLibrary(libPath);

{/*Test dynload block*/
    var lib = null;
    log.info("Test dynload: "+bridjs.Pointer);
    //console.log(libPath); 
    try {
      var lib = bridjs.dl.loadLibrary(libPath);
      //var testMultiplyFunction= bridjs.dl.findSymbol(lib, "testMultiplyFunction");
      log.info(lib);
      log.info("Lib: "+JSON.stringify(lib)+", testMultiplyFunction: "+JSON.stringify(testMultiplyFunction)+", \n\
        symbols: "+bridjs.symbols(libPath));
      /*
      assert(
        symbols.length > 10,
        "Didn't find expected symbols in " + libPath);*/
        log.info("Test dynload pass");
    }catch(e){
       log.error(e); 
    }finally{
        if(lib){
            bridjs.dl.freeLibrary(lib);
        }
    }
    
}
{/*Test dyncall block*/
    log.info("Test dyncall");
    
     var lib = null, vm = null, testMultiplyFunction, dyncall = bridjs.dc, ret, 
     Signature = bridjs.dc.Signature,NativeFunction = bridjs.dc.NativeFunction, nativeFunction,
     startSeconds;
    //console.log(libPath); 
    try {
        lib = bridjs.dl.loadLibrary(libPath);
        testMultiplyFunction= bridjs.dl.findSymbol(lib, "testMultiplyFunction");
        vm = dyncall.newCallVM(4096);
        //log.info("vm pointer: "+JSON.stringify(vm));
        dyncall.mode(vm,bridjs.dc.CALL_C_DEFAULT);
        dyncall.reset(vm);
        startSeconds = Utils.timeSeconds();
        dyncall.reset(vm);
        dyncall.argShort(vm,2);
        dyncall.argInt(vm,2);
        dyncall.argLong(vm,2);
        dyncall.argLongLong(vm,2);
        dyncall.argDouble(vm,2.5);
        ret = dyncall.callDouble(vm,testMultiplyFunction);
        log.info("Spend "+(Utils.timeSeconds()-startSeconds)+" to invoke testMultiplyFunction");
        log.info("2 x 2 x 2 x 2 x 2.5 = "+ret);
        assert(ret===40, "Call testMultiplyFunction fail");
        
        
        assert(Signature.INT32_TYPE==='i', "invalid int32 type");
        assert(Signature.INT_TYPE==='i', "invalid int32 type");
        
        nativeFunction = new NativeFunction( vm, testMultiplyFunction,
        Signature.DOUBLE_TYPE,Signature.INT16_TYPE,Signature.INT32_TYPE,
        Signature.LONG_TYPE,Signature.LONGLONG_TYPE,Signature.DOUBLE_TYPE);
        
        log.info("returnType = "+nativeFunction.getReturnType()+", argumentsLength = "+nativeFunction.getArgumentsLength());
        
        startSeconds = Utils.timeSeconds();
        log.info("2 x 2 x 2 x 2 x 2.5 = "+nativeFunction.call(2,2,2,2,2));
        log.info("Spend "+(Utils.timeSeconds()-startSeconds)+" to invoke testMultiplyFunction");
        /*
        assert(signature.getReturnType()===1, "Get return type fail");
        assert(signature.getArgumentsLength()===6, "Get arguments length fail");
        assert(signature.getArgumentType(2)===3, "Get argument type fail");
        */
        log.info("Test dyncall pass");
    }catch(e){
       log.error(e); 
    }finally{
        
        if(vm){
            bridjs.dc.free(vm);
        }
        
        if(lib){
            bridjs.dl.freeLibrary(lib);
        }
    }
    
}