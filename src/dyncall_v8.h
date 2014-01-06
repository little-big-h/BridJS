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
#pragma once

#include <node.h>

extern "C"
{
	#include "dyncall.h"
}

namespace bridjs{
class Dyncall {
public:
	static v8::Handle<v8::Value> newCallVM(const v8::Arguments& args);
	static v8::Handle<v8::Value> free(const v8::Arguments& args);
	static v8::Handle<v8::Value> reset(const v8::Arguments& args);
	static v8::Handle<v8::Value> mode(const v8::Arguments& args);
	static v8::Handle<v8::Value> argBool(const v8::Arguments& args);
	static v8::Handle<v8::Value> argChar(const v8::Arguments& args);
	static v8::Handle<v8::Value> argShort(const v8::Arguments& args);
	static v8::Handle<v8::Value> argInt(const v8::Arguments& args);
	static v8::Handle<v8::Value> argLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> argLongLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> argFloat(const v8::Arguments& args);
	static v8::Handle<v8::Value> argDouble(const v8::Arguments& args);
	static v8::Handle<v8::Value> argPointer(const v8::Arguments& args);
	static v8::Handle<v8::Value> argStruct(const v8::Arguments& args);

	static v8::Handle<v8::Value> callVoid(const v8::Arguments& args);
	static v8::Handle<v8::Value> callBool(const v8::Arguments& args);
	static v8::Handle<v8::Value> callChar(const v8::Arguments& args);
	static v8::Handle<v8::Value> callShort(const v8::Arguments& args);
	static v8::Handle<v8::Value> callInt(const v8::Arguments& args);
	static v8::Handle<v8::Value> callLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> callLongLong(const v8::Arguments& args);
	static v8::Handle<v8::Value> callFloat(const v8::Arguments& args);
	static v8::Handle<v8::Value> callDouble(const v8::Arguments& args);
	static v8::Handle<v8::Value> callPointer(const v8::Arguments& args);
	static v8::Handle<v8::Value> callStruct(const v8::Arguments& args);

	static v8::Handle<v8::Value> getError(const v8::Arguments& args);

	static v8::Handle<v8::Value> newStruct(const v8::Arguments& args);
	static v8::Handle<v8::Value> structField(const v8::Arguments& args);
	static v8::Handle<v8::Value> subStruct(const v8::Arguments& args);
	static v8::Handle<v8::Value> closeStruct(const v8::Arguments& args);
	//static v8::Handle<v8::Value> structAlignment(const v8::Arguments& args);
	static v8::Handle<v8::Value> freeStruct(const v8::Arguments& args);
	static v8::Handle<v8::Value> structSize(const v8::Arguments& args);
	static v8::Handle<v8::Value> defineStruct(const v8::Arguments& args);
}; // namespace dyncall
}//bridjs
