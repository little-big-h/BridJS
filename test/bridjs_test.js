var assert = require('assert'), bridjs = require('../lib/bridjs.js'), log4js = require("log4js");

var log = log4js.getLogger("bridjs_test");

var libPath = process.cwd() + '/lib/bridjs/' + bridjs.platformTag + '/bridjs.node';
//var lib = bridjs.rawBindings.dl.loadLibrary(libPath);

{/*Test dynload block*/
    var lib = null;
    log.info("Test dynload");
    //console.log(libPath); 
    try {
      var lib = bridjs.dl.loadLibrary(libPath);
      var testMultiplyFunction= bridjs.dl.findSymbol(lib, "testMultiplyFunction");
      console.log("Lib: "+JSON.stringify(lib)+", testMultiplyFunction: "+JSON.stringify(testMultiplyFunction)+", \n\
        symbols: "+bridjs.symbols(libPath));
      /*
      assert(
        symbols.length > 10,
        "Didn't find expected symbols in " + libPath);*/
    }catch(e){
       log.error(e); 
    }finally{
        if(lib){
            bridjs.dl.freeLibrary(lib);
        }
    }
    log.info("Test dynload pass");
}
{/*Test dyncall block*/
    log.info("Test dyncall");
    
     var lib = null, vm = null, testMultiplyFunction, dyncall = bridjs.dc, ret;
    //console.log(libPath); 
    try {
        lib = bridjs.dl.loadLibrary(libPath);
        testMultiplyFunction= bridjs.dl.findSymbol(lib, "testMultiplyFunction");
        vm = dyncall.newCallVM(4096);
        //log.info("vm pointer: "+JSON.stringify(vm));
        dyncall.mode(vm,bridjs.dc.CALL_C_DEFAULT());
        dyncall.reset(vm);
        dyncall.argInt(vm,2);
        dyncall.argDouble(vm,"2.5");
        ret = dyncall.callDouble(vm,testMultiplyFunction);
        log.info("2 x 2.5 = "+ret);
        assert(ret===5, "Call testMultiplyFunction fail");
        
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