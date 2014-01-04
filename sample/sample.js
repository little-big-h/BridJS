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
 * THIS SOFTWARE IS PROVIDED BY OLIVIER CHAFIK, JIA-HAN SU AND CONTRIBUTORS 
 * ``AS IS'' AND ANY * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE 
 * LIABLE FOR ANY * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
var bridjs = require('../lib/bridjs.js'), Signature = bridjs.Signature, 
        libPath = __dirname + "/" + bridjs.LIBRARY_PATH + ".node", nativeModule, 
        ret;

/*
 typedef struct{
    int16_t w;
    int32_t x;
    long y;
    LONGLONG z;
    double e;
 } TestStruct;
 */
var TestStruct = bridjs.defineStruct({
    x: bridjs.structField(Signature.int16, 0),
    y: bridjs.structField(Signature.int32, 1),
    z: bridjs.structField(Signature.long, 2),
    w: bridjs.structField(Signature.longlong, 3),
    e: bridjs.structField(Signature.double, 4)
});
/*
typedef struct{
    double x;
    double y;
} Point2d;
*/
var Point2d = bridjs.defineStruct({
    x : bridjs.structField(Signature.double, 0),
    y : bridjs.structField(Signature.double, 1)
});
/*
typedef struct{
    double x;
    double y;
    double z;
} Point3d;
 */        
var Point3d = bridjs.defineStruct({
    x : bridjs.structField(Signature.double,0),
    y : bridjs.structField(Signature.double,1),
    z : bridjs.structField(Signature.double,2)
});
/*
typedef struct{
    char w;
    TestStruct subStruct;
    int16_t x;
    Point2d point2d;
    int32_t y;
    Point3d point3d;
    int64_t z;
} TestComplexStruct; 
 */
var TestComplexStruct = bridjs.defineStruct({
    w:bridjs.structField(Signature.CHAR_TYPE,0),
    subStruct:bridjs.structField(TestStruct,1),
    x:bridjs.structField(Signature.INT16_TYPE,2),
    point2d:bridjs.structField(Point2d,3),
    y:bridjs.structField(Signature.INT32_TYPE,4),
    point3d:bridjs.structField(Point3d,5),
    z:bridjs.structField(Signature.INT64_TYPE,6)
});

var callbackFunctionDefine = bridjs.defineFunction(Signature.double, 
    Signature.int16, Signature.int32, Signature.long, Signature.longlong, 
    Signature.double);

var callback = bridjs.newCallback(callbackFunctionDefine, function(w, x, y, z, e) {
        console.log("Callback function was invoked");
    
        return w*x*y*z*e;
});

var NativeModule = bridjs.defineModule({
    /*double testMultiplyFunction(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e)*/
    testMultiply: bridjs.defineFunction(Signature.double, Signature.int16, Signature.int32,
            Signature.long, Signature.longlong, Signature.double).bind("testMultiplyFunction"),
            
    /*double testStructFunction(const TestStruct* pTestStruct)*/
    testStructFunction: bridjs.defineFunction(Signature.double, Signature.pointer),
    
    /*double testComplexStructFunction(const TestComplexStruct* pTestStruct)*/
    testComplexStructFunction: bridjs.defineFunction(Signature.double, Signature.pointer),
    
    /*double testArrayStructFunction(const TestArrayStruct* pTestStruct)*/
    testArrayStructFunction: bridjs.defineFunction(Signature.double, callbackFunctionDefine),
    
    /*void testAsyncCallbackFunction(MultiplyCallbackFunction callbackFunction);*/
    testAsyncCallbackFunction: bridjs.defineFunction(Signature.void, Signature.pointer),
    
    /*const TestStruct* testStructPassByPointerFunction(const TestStruct* pTestStruct)*/
    testStructPassByPointerFunction: bridjs.defineFunction(bridjs.byPointer(TestStruct), Signature.pointer),
    
    /*void testStructCallbackFunction(const TestStruct* pTestStruct,TestStructCallbackFunction callbackFunction)*/
    testStructCallbackFunction: bridjs.defineFunction(Signature.void, bridjs.byPointer(TestStruct), Signature.pointer),
    
    /*const double* testValuePassByPointerFunction(const double *returnValue)*/
    testValuePassByPointerFunction: bridjs.defineFunction(bridjs.byPointer(bridjs.NativeValue.double), bridjs.byPointer(bridjs.NativeValue.double))
}, libPath);

nativeModule = new NativeModule();

console.log(nativeModule.testMultiply(2,2,2,2,2.5));

var testComplexStruct = new TestComplexStruct();
testComplexStruct.w = testComplexStruct.x = testComplexStruct.y = testComplexStruct.z = 2;
testComplexStruct.subStruct.e =  testComplexStruct.point2d.x = testComplexStruct.point3d.y = 2.5;

console.log(testComplexStruct.point3d.y);
console.log(nativeModule.testComplexStructFunction(bridjs.byPointer(testComplexStruct)));

bridjs.async(nativeModule).testMultiply(2,2,2,2,2.5, function(returnValue){
    console.log("return value: "+returnValue);
});

nativeModule.testAsyncCallbackFunction(callback);

var nativeDouble = new bridjs.NativeValue.double(2.5);
var returnNativeDouble = nativeModule.testValuePassByPointerFunction(bridjs.byPointer(nativeDouble));

console.log(nativeDouble.get());