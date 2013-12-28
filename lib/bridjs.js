/*
 * BridJS - Dynamic and blazing-fast native interop for JavaScript.
 * https://github.com/jiahansu/BridJS
 *
 * Copyright (c) 2013-2013, Olivier Chafik (http://ochafik.com/) and Jia-Han Su (https://github.com/jiahansu)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Olivier Chafik nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY OLIVIER CHAFIK AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
var my = require('myclass'), BridjsException = require("./bridjs_exception"),
        CallbackWrapper = require("./callback_wrapper"),Utils = require("./utils"),
        StructField = require("./struct_field"),
        bridjs, platformTag = process.platform + '_' + process.arch,
        libraryPath = "../" + platformTag + "/bridjs", NativeFunctionWrapper,
        SignatureWrapper, defaultStackSize = 4096, Struct, structImplMap={};




bridjs = loadPlatformTaggedNativeLibrary();

if (bridjs) {
    function loadPlatformTaggedNativeLibrary() {
        //var subPath = name + '/' + process.platform + '_' + process.arch + '/' + name
        //return require('./dist/' + subPath) 
        return require(libraryPath);
    }

    function mergeNativeArguments(klass, argsObject) {
        var args = new Array(1 + argsObject.length);

        args[0] = klass._bridjs_impl_vm;
        for (j = 0; j < argsObject.length; ++j) {
            args[j + 1] = argsObject[j];
        }

        return args;
    }

    function setNativeFunction(klass, name) {
        var nativeFunction, callMethod, args,
                dyncall = bridjs.dc, dynload = bridjs.dl, symbol =
                dynload.findSymbol(klass._bridjs_impl_lib, name), j,
                property = klass.prototype[name], callAsyncMthod;

        if (symbol) {
            args = new Array(2 + property.getArgumentLength());
            args[0] = symbol;
            args[1] = property.getReturnType();
            for (j = 0; j < property.getArgumentLength(); ++j) {
                args[j + 2] = property.getArgumentType(j);
            }

            nativeFunction = new dyncall.NativeFunction(args);
            callMethod = dyncall.NativeFunction.prototype.call;
            callAsyncMthod = dyncall.NativeFunction.prototype.callAsync;
            klass.prototype[name] = function() {
                return callMethod.apply(nativeFunction,  mergeNativeArguments(klass, arguments));
            };
            klass.prototype._bridjs_impl_async[name] = function() {
                var args = mergeNativeArguments(klass, arguments),
                        lastIndex =args.length-1,callback =args[lastIndex]; 
                args[lastIndex] = new CallbackWrapper(callback);
                
                return callAsyncMthod.apply(nativeFunction,  args);
            };
        } else {
            throw new BridjsException("Fail to locate function: " + name);
        }
    }
    
    function setStructField(instance,structImpl,buffer, structField, order){
        
        Object.defineProperty(instance, structField.getName(),{
            get: function(){
                return structImpl.getField(order, buffer);
            },
            set: function(value){
                structImpl.setField(order, value, buffer);
            }
        });
    }
    
    SignatureWrapper = my.Class(bridjs.dc.Signature, {
        constructor: function(args) {
            if (args.length >= 1) {
                this.returnType = Utils.checkType(args[0]);
                this.argumentTypes = new Array(args.length - 1);

                for (var i = 1; i < args.length; ++i) {
                    this.argumentTypes[i - 1] = Utils.checkType(args[i]);
                }
            } else {
                throw new BridjsException("Illegal agguments to describe a valid function signature");
            }
        },
        getReturnType: function() {
            return this.returnType;
        },
        getArgumentLength: function() {
            return this.argumentTypes.length;
        },
        getArgumentType: function(index) {
            if (index >= 0 && index < this.argumentTypes.length) {
                return this.argumentTypes[index];
            } else {
                throw new BridjsException("OutOfBoundary, length = " + this.argumentTypes.length + ", index = " + index);
            }
        }
    });
    
    Struct = my.Class({
        constructor:function(){
            var name, field, array = [], types, signature="", i, structImpl, buffer;

            for(name in this){
                field = this[name];
                
                if(field instanceof StructField){
                    field.setName(name);
                    array.push(field);
                }
            }
            array.sort(StructField.compare);
            types = new Array(array.length);
            
            for(i=0;i<types.length;++i){
                types[i] = array[i].getType();
                signature+=types[i];
            }
            /*Reuse Native Struct object*/
            structImpl = structImplMap[signature];
            if(typeof(structImpl)!=="object"){
                structImpl = new bridjs.dc.Struct(types);
                structImplMap[signature] = structImpl;
            }
            //console.log(signature);
            buffer = new Buffer(structImpl.getSize());
            
            for(i=0;i<array.length;++i){
                field = array[i];
                setStructField(this,structImpl,buffer,field,i);
            }
            
            this.getPointer = function(){
                return buffer;
            };
        }
    });
    
    module.exports = my.Class({
        STATIC: {
            rawBindings: bridjs,
            dyncall: bridjs.dc,
            dynload: bridjs.dl,
            dyncallback: bridjs.dcb,
            dc: bridjs.dc,
            dl: bridjs.dl,
            dcb: bridjs.dcb,
            test: bridjs.test,
            Pointer: bridjs.dc.Pointer,
            Struct: Struct,
            utils: bridjs.utils,
            DEFAULT_STACK_SIZE: defaultStackSize,
            LIBRARY_PATH: libraryPath,
            symbols: function(libPath) {
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
            },
            register: function(klass, libraryPath, config) {
                var propertyNames, property, dyncall = bridjs.dc,
                        dynload = bridjs.dl, name;
                try {
                    if (typeof (config) !== "object") {
                        config = {};
                    }

                    if (typeof (config.statckSize) !== "number"
                            || config.statckSize <= 0) {
                        config.stackSize = defaultStackSize;
                    }
                    //console.log(config);
                    klass._bridjs_impl_vm = dyncall.newCallVM(config.stackSize);
                    klass._bridjs_impl_lib = dynload.loadLibrary(libraryPath);
                    if (typeof (klass.prototype._bridjs_impl_async) !== "object") {
                        klass.prototype._bridjs_impl_async = {};
                    }
                    propertyNames = Object.getOwnPropertyNames(klass.prototype);
                    //console.log(propertyNames);
                    for (i = 0; i < propertyNames.length; ++i) {
                        name = propertyNames[i];
                        property = klass.prototype[name];
                        //console.log(name+", "+property);
                        if (property instanceof SignatureWrapper) {
                            setNativeFunction(klass, name);
                        }
                    }
                } catch (e) {
                    this.unregister(klass);
                    throw e;
                }
            },
            unregister: function(klass) {
                if (klass._bridjs_impl_vm) {
                    bridjs.dc.free(klass._bridjs_impl_vm);
                    klass._bridjs_impl_vm = null;
                }

                if (klass._bridjs_impl_lib) {
                    bridjs.dl.freeLibrary(klass._bridjs_impl_lib);
                    klass._bridjs_impl_lib = null;
                }
            },
            defineFunction: function() {
                return new SignatureWrapper(arguments);
            },
            async: function(object){
                return object._bridjs_impl_async;
            },
            structField:function(type, order){
                return new StructField(type, order);
            }
        }
    });

    //console.log(bridjs);
} else {
    throw new BridjsException("Fail to initalize bridjs from: " + libarryPath);
}

