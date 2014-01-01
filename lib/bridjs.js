/*
 * BridJS - Dynamic and blazing-fast native interop for JavaScript.
 * https://github.com/jiahansu/BridJS
 *
 * Copyright (c) 2013-2013, Olivier Chafik (http://ochafik.com/) 
 * and Jia-Han Su (https://github.com/jiahansu/BridJS)
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
 * THIS SOFTWARE IS PROVIDED BY OLIVIER CHAFIK, JIA-HAN SU AND CONTRIBUTORS ``AS IS'' AND ANY
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
        CallbackWrapper = require("./callback_wrapper"),
        FunctionCallbackWrapper = require("./function_callback_wrapper"),
        Utils = require("./utils"), StructField = require("./struct_field"),
        PassTypeWrapper = require("./pass_type_wrapper"),
        bridjs, platformTag = process.platform + '_' + process.arch,
        libraryPath = "../" + platformTag + "/bridjs", NativeFunctionWrapper,
        SignatureWrapper, defaultStackSize = 4096, Struct, ArrayStruct,
        structImplMap = {};




bridjs = loadPlatformTaggedNativeLibrary();

if (bridjs) {
    function loadPlatformTaggedNativeLibrary() {
        //var subPath = name + '/' + process.platform + '_' + process.arch + '/' + name
        //return require('./dist/' + subPath) 
        return require(libraryPath);
    }
    /*
     function mergeNativeArguments(argsObject) {
     var args = new Array(argsObject.length),j;
     
     //args[0] = klass._bridjs_impl_vm;
     for (j = 0; j < argsObject.length; ++j) {
     args[j] = argsObject[j];
     }
     
     return args;
     }*/
    function getNativeType(klass) {
        var type;
        
        if(klass instanceof SignatureWrapper){
            type = bridjs.dc.Signature.POINTER_TYPE;
        }else if (klass instanceof PassTypeWrapper) {
            type = klass.getPassType();
        } else {
            type = klass;
        }

        return type;
    }
    function setNativeFunction(klass, name) {
        var nativeFunction, callMethod, args,
                dyncall = bridjs.dc, dynload = bridjs.dl, symbol =
                dynload.findSymbol(klass._bridjs_impl_lib, name), j,
                property = klass.prototype[name], callAsyncMthod, wrapReturn,
                returnType = property.getReturnType();

        if (symbol) {
            args = new Array(2 + property.getArgumentLength());
            args[0] = symbol;
            args[1] = getNativeType(returnType);
            for (j = 0; j < property.getArgumentLength(); ++j) {
                args[j + 2] = getNativeType(property.getArgumentType(j));
            }

            nativeFunction = new dyncall.NativeFunction(args);
            callMethod = dyncall.NativeFunction.prototype.call;
            callAsyncMthod = dyncall.NativeFunction.prototype.callAsync;

            if (returnType instanceof PassTypeWrapper) {
                /*Wrap pointer to struct*/
                var type = returnType.getType();

                wrapReturn = function(value) {
                    return new type(value);
                };
            } else {
                wrapReturn = function(value) {
                    return value;
                };
            }

            klass.prototype[name] = function() {
                return wrapReturn(callMethod.call(nativeFunction, klass._bridjs_impl_vm,
                        arguments));
            };
            klass.prototype._bridjs_impl_async[name] = function() {
                var lastIndex = arguments.length - 1, callback = arguments[lastIndex];
                arguments[lastIndex] = new CallbackWrapper(callback, returnType);
                //console.log(arguments);
                return callAsyncMthod.call(nativeFunction, klass._bridjs_impl_vm,
                        arguments);
            };
        } else {
            throw new BridjsException("Fail to locate function: " + name);
        }
    }

    function setStructField(instance, structImpl, structField, order, bufferWrapper, subStruct) {

        if (subStruct) {
            Object.defineProperty(instance, structField.getName(), {
                get: function() {
                    return subStruct;
                },
                set: function(value) {
                    module.exports.getStructPointer(value).copy(
                            module.exports.getStructPointer(subStruct));
                }
            });
        } else {
            Object.defineProperty(instance, structField.getName(), {
                get: function() {
                    return structImpl.getField(order, bufferWrapper.buffer);
                },
                set: function(value) {
                    structImpl.setField(order, value, bufferWrapper.buffer);
                }
            });
        }
    }

    SignatureWrapper = my.Class(bridjs.dc.Signature, {
        constructor: function(args) {
            var returnType, argumentTypes, i;
            if (args.length >= 1) {
                returnType = Utils.checkType(args[0]);
                argumentTypes = new Array(args.length - 1);

                for (i = 1; i < args.length; ++i) {
                    argumentTypes[i - 1] = Utils.checkType(args[i]);
                }

                this.getReturnType = function() {
                    return returnType;
                };
                this.getArgumentLength = function() {
                    return argumentTypes.length;
                };
                this.getArgumentType = function(index) {
                    if (index >= 0 && index < argumentTypes.length) {
                        return argumentTypes[index];
                    } else {
                        throw new BridjsException("OutOfBoundary, length = " + argumentTypes.length + ", index = " + index);
                    }
                };
            } else {
                throw new BridjsException("Illegal agguments to describe a valid function signature");
            }
        }
    });

    Struct = my.Class({
        STATIC: {
            allowToAllocateBuffer: true
        },
        constructor: function(pointer) {
            var name, field, array = [], types, signature, i,
                    type, subStruct, subStructs = {}, structImpl, buffer,
                    bufferWrapper = {};

            for (name in this) {
                field = this[name];

                if (field instanceof StructField) {
                    field.setName(name);
                    array.push(field);
                }
            }
            array.sort(StructField.compare);
            types = new Array(array.length);
            signature = new Array(types.length);

            if (array.length > 0) {
                for (i = 0; i < array.length; ++i) {
                    type = array[i].getType();
                    //console.log(i+", "+type+", "+typeof(type));
                    if (typeof (type) === "string") {
                        types[i] = type;
                    } else {
                        /*We will assign buffer to sub struct later*/
                        Struct.allowToAllocateBuffer = false;
                        subStruct = new type();
                        Struct.allowToAllocateBuffer = true;
                        types[i] = subStruct._bridjs_impl_get_struct_impl();
                        subStructs[i] = subStruct;
                    }

                    /*Type will be a string or a bridjs.dyncall.Struct object*/
                    signature.push(types[i].toString());
                }

                signature = signature.join('');
                //console.log(signature);
                /*Reuse Native Struct object*/
                structImpl = structImplMap[signature];
                if (typeof (structImpl) !== "object") {
                    structImpl = new bridjs.dc.Struct(types);
                    structImplMap[signature] = structImpl;
                }
                if (typeof (pointer) === "object") {
                    buffer = pointer;
                } else if (Struct.allowToAllocateBuffer) {
                    buffer = new Buffer(structImpl.getSize());
                } else {
                    buffer = null;
                }
                //console.log(this._bridjs_impl_buffer);
                for (i = 0; i < array.length; ++i) {
                    field = array[i];
                    setStructField(this, structImpl, field, i, bufferWrapper, subStructs[i]);
                }
            }

            this._bridjs_impl_get_struct_impl = function() {
                return structImpl;
            };
            this._bridjs_impl_get_size = function() {
                return structImpl.getSize();
            };
            this._bridjs_impl_get_sub_struct = function() {
                return subStructs;
            };

            this._bridjs_impl_get_pointer = function() {
                return bufferWrapper.buffer;
            };
            this._bridjs_impl_update_buffer = function(buffer) {
                if (typeof (bufferWrapper.buffer) !== "object") {
                    var subStructs = this._bridjs_impl_get_sub_struct(), index,
                            subStruct, structImpl = this._bridjs_impl_get_struct_impl(),
                            start, end;

                    bufferWrapper.buffer = buffer;

                    for (index in subStructs) {
                        subStruct = subStructs[index];
                        start = structImpl.getFieldOffset(index);
                        end = start + subStruct._bridjs_impl_get_size();
                        //console.log(index + ", " + start+", "+end);
                        subStruct._bridjs_impl_update_buffer(bufferWrapper.buffer.slice(start, end));
                    }
                } else {
                    throw new BridJSException("Buffer was allocated: " + this._bridjs_impl_buffer);
                }
            };

            if (Struct.allowToAllocateBuffer) {
                this._bridjs_impl_update_buffer(buffer);
            }
            /*
             this.toString= function(){
             return structImpl.toString();
             };*/
        }
    });

    ArrayStruct = my.Class(Struct, {
        constructor: function(type, length) {
            var signature, structImpl, bufferWrapper = {};

            ArrayStruct.Super.call(this);
            //console.log(length);
            signature = [type, '(' + length + ')'].join('');
            //console.log(signature);
            /*Reuse Native Struct object*/
            structImpl = structImplMap[signature];
            if (typeof (structImpl) !== "object") {
                structImpl = new bridjs.dc.ArrayStruct(type, length);
                structImplMap[signature] = structImpl;
            }
            if (Struct.allowToAllocateBuffer) {
                buffer = new Buffer(structImpl.getSize());
            } else {
                buffer = null;
            }

            this._bridjs_impl_get_struct_impl = function() {
                return structImpl;
            };
            this._bridjs_impl_get_size = function() {
                return structImpl.getSize();
            };
            this._bridjs_impl_get_pointer = function() {
                return bufferWrapper.buffer;
            };
            this._bridjs_impl_update_buffer = function(buffer) {
                if (typeof (bufferWrapper.buffer) !== "object") {
                    bufferWrapper.buffer = buffer;
                } else {
                    throw new BridJSException("Buffer was allocated: " + this._bridjs_impl_buffer);
                }
            };
        },
        get: function(index) {
            var buffer = module.exports.getStructPointer(this);

            return this._bridjs_impl_get_struct_impl().getField(index, buffer);
        },
        set: function(index, value) {
            var buffer = module.exports.getStructPointer(this);

            return this._bridjs_impl_get_struct_impl().setField(index, value, buffer);
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
            Signature: bridjs.dc.Signature,
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
            async: function(object) {
                return object._bridjs_impl_async;
            },
            structField: function(type, order) {
                return new StructField(type, order);
            },
            structArrayField: function(type, length, order) {
                var structClass = my.Class(ArrayStruct, {
                    constructor: function() {
                        structClass.Super.call(this, type, length);
                    }
                });

                return module.exports.structField(structClass, order);
            },
            defineStruct: function(elements) {
                var structClass;

                elements.constructor = function(pointer) {
                    structClass.Super.call(this, pointer);
                };

                structClass = my.Class(Struct, elements);

                return structClass;
            },
            byPointer: function(klass) {
                if(klass instanceof Struct){
                    /*Struct's instance, return pointer immediately*/
                    return module.exports.getStructPointer(klass);
                }else{
                    /*Struct's class, return PassTypeWrapper*/
                  return new PassTypeWrapper(klass, bridjs.dc.Signature.POINTER_TYPE);  
                } 
            },
            getStructPointer: function(struct) {
                return struct._bridjs_impl_get_pointer();
            },
            getStructSize: function(struct) {
                return struct._bridjs_impl_get_size();
            },
            newCallback: function(functionDefinition, callbackFunction) {
                if (functionDefinition instanceof SignatureWrapper) {
                    var returnType = functionDefinition.getReturnType(),
                            callbackWrapper, argumentTypes = 
                            new Array(functionDefinition.getArgumentLength()),
                            nativeArgumentTypes = 
                            new Array(functionDefinition.getArgumentLength()), i, 
                            type;

                    for (i = 0; i < functionDefinition.getArgumentLength(); ++i) {
                        type = functionDefinition.getArgumentType(i);
                        argumentTypes[i] = type;
                        nativeArgumentTypes[i] = getNativeType(type );
                    }
                    
                    callbackWrapper = new FunctionCallbackWrapper(callbackFunction,
                    argumentTypes);
                    
                    return bridjs.dcb.newCallback(returnType, nativeArgumentTypes, 
                            callbackWrapper);
                } else {
                    throw new BridjsException("Illegal argument exception, unknown function definition: "
                            + functionDefinition);
                }
            },
            defineModule: function(properties, libraryPath, config) {
                var klass = my.Class(properties);

                module.exports.register(klass, libraryPath, config);

                return klass;
            }
        }
    });

    //console.log(bridjs);
} else {
    throw new BridjsException("Fail to initalize bridjs from: " + libarryPath);
}

