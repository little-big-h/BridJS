/*
Load with:

  var bridjs = require('./bridjs')
  var clib = bridjs.lib("c")
*/

var bridjs;

exports.platformTag = process.platform + '_' + process.arch;

function loadPlatformTaggedNativeLibrary(name) {
  //var subPath = name + '/' + process.platform + '_' + process.arch + '/' + name
  //return require('./dist/' + subPath)
  return require('./' + name + '/' + exports.platformTag + '/' + name);
}

bridjs = loadPlatformTaggedNativeLibrary('bridjs');

if(bridjs){
    exports.rawBindings = bridjs;
    exports.dyncall = exports.dc = bridjs.dc;
    exports.dynload = exports.dl = bridjs.dl;
    exports.dyncallback = exports.dcb = bridjs.dcb;
    exports.test = bridjs.test;
    exports.Pointer = bridjs.dc.Pointer;

    exports.symbols = function(libPath) {
      var symbols = new Array();
      var syms = bridjs.dl.symsInit(libPath);
      try {
        var count = bridjs.dl.symsCount(syms);
        for (var i = 0; i < count; i++) {
          var name = bridjs.dl.symsName(syms, i);
          if (name)
            symbols.push(name);
        }
      } finally {
        bridjs.dl.symsCleanup(syms);
      }
      return symbols;
    };


    exports.lib = function(name) {
      var lib = libs[name];
      if (!lib) {
        // TODO path stuff
        console.log('TODO: load native library with dynload');

        var lib = bridjs.dcLoadLibrary(name);
        var syms = bridjs.dcSymsInit(name);

        var functions = {};

        lib = libs[name] = {
          close: function() {
            bridjs.dcCloseLibrary(lib);
            bridjs.dcCloseSyms(syms);
          },
          symbols: [ 'TODO' ],
          func: function(name, signature) {
            var f = functions[name];
            if (!f) {
              console.log('TODO: load native library with dynload');
              var pointer = bridjs.dcSymGet(syms, name);
              if (!pointer)
                pointer = bridjs.dcSymGet(syms, '_' + name);

              f = functions[name] = function() {
                console.log('TODO: dccall(' + arguments + ')');
              };
            }
            return f;
          }
        };
      }
      return lib;
    };
}else{
    throw "Fail to load native library from: "+'./' + name + '/' + exports.platformTag + '/' + "bridjs";
}

