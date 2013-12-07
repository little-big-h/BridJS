var assert = require('assert'), bridjs = require('../lib/bridjs.js'), log4js = require("log4js")

var log = log4js.getLogger("bridjs_test");

var libPath = process.cwd() + '/lib/bridjs/' + bridjs.platformTag + '/bridjs.node';
//var lib = bridjs.rawBindings.dl.loadLibrary(libPath);

{/*Test dynload block*/
    log.info("Test dynload");
    //console.log(libPath); 
    try {
      var symbols = bridjs.symbols("kernel32");
     // console.log(JSON.stringify(symbols));
      assert(
        symbols.length > 10,
        "Didn't find expected symbols in " + libPath);
    }catch(e){
       log.error(e); 
    }
    log.info("Test dynload pass");
}
{/*Test dyncall block*/
    log.info("Test dyncall");
    var vm = bridjs.dc.newCallVM(4096);
    log.info("vm pointer: "+bridjs.dc.CALL_C_DEFAULT());
    bridjs.dc.mode(vm,bridjs.dc.CALL_C_DEFAULT());
    bridjs.dc.reset(vm);
    bridjs.dc.free(vm);
    log.info("Test dyncall pass");
}