---
name: quantum-script--base16
description: >-
  How to use the Quantum Script Base16 extension (quantum-script--base16),
  the hexadecimal codec loaded with Script.requireExtension("Base16"):
  Base16.encode(str) (bytes -> uppercase hex, never fails, non-strings are
  encoded as their text: encode(255) is "323535"), Base16.decode(str)
  (strict: even length, only 0-9 a-f A-F, otherwise undefined; returns a
  String of bytes) and Base16.decodeToBuffer(str) (same, returns a Buffer);
  testing failure with Script.isUndefined (decode("") is "" which is falsy);
  differences from Buffer.fromHex / toHex (lowercase, lenient); Base32 /
  Base64 siblings; which hosts have it (quantum-script, magnet; NOT
  fabricare build scripts); the C++ side (registerInternalExtension together
  with Buffer, initExecutive, quantumScriptExtension entry point,
  XYO::Encoding::Base16). Use when writing or reviewing Quantum Script code
  that converts bytes to / from hex, C++ code that includes
  <XYO/QuantumScript.Extension/Base16.hpp>, a fabricare.json depending on
  "quantum-script--base16", or when working inside the quantum-script--base16
  repository.
---

# quantum-script--base16

Hexadecimal (Base16, RFC 4648) codec extension of Quantum Script (see the
`quantum-script` skill for the language and its differences from
JavaScript, and the `quantum-script--buffer` skill for the `Buffer` type;
their rules apply). Purpose: **turn binary data into text and back** —
digests, keys, tokens, binary file fragments — for logs, JSON, config files
and command lines, with strict validation on input.

Full documentation: `docs/` in the quantum-script--base16 repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--base16\docs` on this machine):
README (purpose), getting-started (build, load, hosts, C++ registration),
**script-api** (exact behavior, argument conversion, recipes), cpp-api,
reference. When in doubt read
`source/XYO/QuantumScript.Extension/Base16/Library.cpp` (~75 lines) and
`XYO::Encoding::Base16` in the xyo-encoding repository.

## Script API

```javascript
Script.requireExtension("Base16");          // also loads Buffer

Base16.encode("foo");                       // "666F6F"  uppercase, 2 chars per byte, never fails
Base16.encode(buffer);                      // hex of the first buffer.length bytes
Base16.decode("666f6F");                    // "foo"     String of bytes, or undefined
Base16.decodeToBuffer("00FF10");            // Buffer (size == length == 3), or undefined
```

## Hard rules

1. **Decoding is strict, failure is `undefined`.** Valid input: even
   length, only `0-9 a-f A-F`. Odd length, spaces, newlines, `0x`, `:` or
   any other character → `undefined`, no exception, no partial result.
   Strip separators yourself before decoding.
2. **Test failure with `Script.isUndefined(r)`**, never `!r`:
   `Base16.decode("")` is `""` and `Base16.decodeToBuffer("")` is an empty
   Buffer — both falsy but valid.
3. **The argument is converted with `toString` first.** `encode(255)` is
   `"323535"` (the text "255"), not `"FF"`; `encode()` encodes the text
   `"undefined"`; `decode(4142)` is `"AB"`. To encode a byte value, put it
   in a buffer: `var b = Buffer(1); b.setU8(0, 255); Base16.encode(b);`.
4. **Bytes, not characters.** `Base16.encode("ă")` is `"C483"` (UTF-8);
   `decode` keeps zero bytes and does not validate UTF-8. Use
   `decodeToBuffer` when the result is binary.
5. **Output is uppercase; input is case-insensitive.** `Buffer.toHex()` and
   `SHA256.hash()` are lowercase — compare after
   `toUpperCaseASCII()` / `toLowerCaseASCII()` (there is no `toUpperCase`).
6. **`Buffer.fromHex` is not a validator**: it maps invalid digits to `0`
   and ignores an odd last character. Use `Base16.decodeToBuffer` for
   untrusted input.
7. **Not available in fabricare build scripts.** fabricare is a static host
   without `Base16`; `requireExtension("Base16")` throws
   `Unable to open "Base16"` there. Use `Buffer.fromString(s).toHex()` /
   `Buffer.fromHex(h)` instead. Available in the `quantum-script`
   interpreter (DLL), `magnet`, and hosts that register it.
8. `JSON.encode` turns a Buffer into `null`: store `Base16.encode(buffer)`
   and restore with `Base16.decodeToBuffer`.
9. Same three functions exist in `Base32` and `Base64`; keep usage
   symmetric when switching codecs.
10. One engine per thread: each thread requires `Base16` itself; the
    functions are stateless.

## Recipes

```javascript
Base16.encode(SHA256.hashToBuffer(data));               // 64 uppercase hex digits
var key = Base16.decodeToBuffer(text);                  // validate a 16 byte hex key
if (Script.isUndefined(key) || key.length != 16) { throw "expected 32 hex digits"; };
Shell.filePutContentsBuffer("magic.bin", Base16.decodeToBuffer("89504E470D0A1A0A"));
JSON.encode({key: Base16.encode(key)});                 // binary in JSON
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/Base16.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                 // host init callback
	Extension::Buffer::registerInternalExtension(executive);  // Base16 requires Buffer
	Extension::Base16::registerInternalExtension(executive);  // scripts still requireExtension("Base16")
};

// without the script engine:
XYO::Encoding::String hex = XYO::Encoding::Base16::encode(bytes);   // uppercase
XYO::Encoding::String out;
bool ok = XYO::Encoding::Base16::decode(hex, out);                 // strict
```

- fabricare.json dependency: `"quantum-script--base16"` (`dll-or-lib`: DLL
  on dynamic platforms, static lib on `*.static` platforms). It pulls in
  `quantum-script`, `quantum-script--console`, `quantum-script--buffer`.
- DLL entry point `extern "C" quantumScriptExtension(Executive *, void *)`
  only with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` and without
  `XYO_QUANTUMSCRIPT_EXTENSION_BASE16_LIBRARY`. Static hosts must register
  the extension as internal.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console, Buffer and Base16 as internal extensions and checks the
  RFC 4648 vectors in `test/test.01.js`; run `make` first),
  `fabricare install` (see the `fabricare` skill). `quantum-script`,
  `quantum-script--console` and `quantum-script--buffer` must be installed
  first.
- Native functions live in `Base16/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`
  and are registered in `initExecutive` with
  `executive->setFunction2("Base16.name(args)", name)`.
- New functions: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `test/test.01.js` and this skill; keep the API in
  step with quantum-script--base32 / --base64.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
