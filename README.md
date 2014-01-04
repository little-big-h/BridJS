BridJS
======
BridJS is a [BridJ](http://bridj.googlecode.com)-like API for runtime binding C function and struct without writing any extra native code.

###Key features
* BridJS binds native function at runtime, you never need to compile any extra native code 
* Uncompromized speed and power by [dyncall](http://www.dyncall.org/)
* Support implicit type wrapping/unwrapping (struct<->pointer and string<->number etc... )
* Support complex struct type (sub-struct and array field), and access theme straightforwadly.
* Execute any native function either by synchronous or by asynchronous way
* Whatever the native callbacks are invoked by any other thread, BridJS always forward callback to V8's default thread 

###Limitation
Like [BridJ](http://bridj.googlecode.com), BridJS also has some limitations:
* Pass structs by value not supported yet (neither as function arguments nor as function return values)
* BridJS does **not support** C++, COM, Objective-C...

###Requirement
* [nodejs](http://nodejs.org/) v0.10.22 or higher
* Windows with [Visual C++ Redistributable for Visual Studio 2012](http://www.microsoft.com/en-us/download/details.aspx?id=30679) or Linux

###Installation
``` bash
npm install bridjs
```

###Tutorial
####1. C function
-------------
If C code is something like this:
``` bash
double testMultiplyFunction(const int16_t w, const int32_t x,const long y, const LONGLONG z, const double e);
``` 

You can define JavaScript prototype like this:
``` bash
var bridjs = require('../lib/bridjs.js'), Sig = bridjs.Signature;

var NativeModule = bridjs.defineModule({
    testMultiplyFunction:  bridjs.defineFunction(Sig.double, Sig.int16, Sig.int32,
            Sig.long, Sig.longlong, Sig.double)
    }, libraryPath);
    
var nativeModule = new NativeModule();

var result = nativeModule.testMultiplyFunction(2,2,2,2,2.5);
``` 
Bind C function API
``` bash
bridjs.defineModule({
  functionName1: bridjsDefineFunction(returnType, arg0Type, arg2Type...),
  functionName2: bridjsDefineFunction(returnType, arg0Type, arg2Type...),
  ...
},libraryFile);
``` 
####2. C struct
---------------
If C code is something like this:
``` bash
typedef struct{
  double x;
  double y;
  double z;
} Point3d;

tydef struct{
  char x;
  Point3d y;
  char str[10];
} ComplexStruct

double testComplexStructFunction(const ComplexStruct* pTestStruct)
``` 	
You can define JavaScript prototype like this:
``` bash
var Point3d = defineStruct({
    x : bridjs.structField(Sig.double,0),
    y : bridjs.structField(Sig.double,1),
    z : bridjs.structField(Sig.double,2)
});

var ComplexStruct = bridjs.defineStruct({
    x : bridjs.structField(Sig.char,0),
    y : bridjs.structField(Point3d,1),
    str : bridjs.structArrayField(Sig.char,10,2)
});

var NativeModule = bridjs.defineModule({
    testComplexStructFunction(:  bridjs.defineFunction(Sig.double, bridjs.byPointer(ComplexStruct))
    }, libraryPath);

var complexStruct = new ComplexStruct();
var nativeModule = new NativeModule();

complexStruct.x = 's';
complexStruct.y.x = 2;
complexStruct.str.set(3) = 's';

var result = nativeModule.testComplexStructFunction(bridjs.byPointer(complexStruct));

``` 
Bind C struct API
``` bash
bridjs.defineStruct({
    element1 : bridjs.structField(elementType,order),
    element2 : bridjs.structField(elementType,order),
    element3 : bridjs.structArrayField(arrayType,arrayLength,order)
    ...
});
``` 
