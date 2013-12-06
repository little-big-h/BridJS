var assert = require('assert');
var bridjs = require('./bridjs.js');

var libPath = process.cwd() + '/bridjs/' + bridjs.platformTag + '/bridjs.node';
var lib = bridjs.rawBindings.loadLibrary(libPath);

//console.log(libPath); 
try {
  var symbols = bridjs.symbols(libPath);
  console.log('Found ' + symbols.length + ' symbols');
  assert(
    symbols.length > 10,
    "Didn't find expected symbols in " + libPath)
}catch(e){
   console.log(e); 
}finally {
  console.log(bridjs.rawBindings.freeLibrary(lib));
}

