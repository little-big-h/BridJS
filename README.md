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
