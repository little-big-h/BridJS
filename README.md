BridJS
======
BridJS is a [BridJ](http://bridj.googlecode.com)-like API for runtime binding C function and struct without writing any extra native code.

##Key features
* BridJS is create binding at runtime, you never need to compile any extra native code 
* Uncompromized speed and power by [dyncall](http://www.dyncall.org/)
* Support implicit type wrapping/unwrapping (struct<->pointer and string<->number etc... )
* Support complex struct tyoe (sub-struct, array field), and straitforward to access struct elements, jsut like C language.
* Execute any native function either by synchronous or by asynchronous way
* Whatever the native callbacks are invoked by any other thread, BridJS always forward callback to V8's default thread  
