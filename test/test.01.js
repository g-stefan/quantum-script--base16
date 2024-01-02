// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("Base16");

var checkEncoding = [
		["", ""],
		["f", "66"],
		["fo", "666F"],
		["foo", "666F6F"],
		["foob", "666F6F62"],
		["fooba", "666F6F6261"],
		["foobar", "666F6F626172"]
	];

var allOk = true;
for(var k of checkEncoding) {
	if(Base16.encode(k[0]) != k[1]) {
		allOk = false;
		Console.writeLn("Error-Encode: " + k[0] + " : " + k[1] + " : " + Base16.encode(k[0]));
	};
};

for(var k of checkEncoding) {
	if(Base16.decode(k[1]) != k[0]) {
		allOk = false;
		Console.writeLn("Error-Decode: " + k[1] + " : " + k[0] + " : " + Base16.decode(k[1]));
	};
};

if(allOk) {
	Console.writeLn("-> test 0001 ok");
};
