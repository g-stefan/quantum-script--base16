# Quantum Script Extension Base16 — Documentation

`quantum-script--base16` is the **hexadecimal (Base16) encoding extension of
Quantum Script**. Loaded with `Script.requireExtension("Base16")`, it adds a
`Base16` object to scripts with three functions:

```javascript
Base16.encode(str);           // bytes  -> "48656C6C6F" (uppercase hex)
Base16.decode(str);           // hex    -> String of bytes, undefined if invalid
Base16.decodeToBuffer(str);   // hex    -> Buffer of bytes, undefined if invalid
```

Base16 (RFC 4648 section 8) writes every byte as two hexadecimal digits. It
is the simplest way to put binary data — hashes, keys, random tokens, file
fragments, anything with zero bytes or non UTF-8 content — into text: logs,
JSON, configuration files, URLs, command lines.

- **Encode bytes, not characters.** The argument is converted to a string
  and each of its bytes becomes two digits: `"ă"` (UTF-8 `c4 83`) encodes
  to `"C483"`. A `Buffer` argument is encoded byte for byte.
- **Uppercase out, either case in.** `encode` always writes `0-9 A-F`;
  `decode` accepts `0-9 a-f A-F`.
- **Strict decoding.** An odd number of digits or any other character
  (spaces, `0x`, separators) makes `decode` return `undefined`. There is no
  partial result.
- **Two output types.** `decode` returns a `String` (bytes kept as they
  are), `decodeToBuffer` a `Buffer` from the `Buffer` extension, which the
  binary APIs (`File.writeFromBuffer`, `Crypt.decrypt`, ...) expect.

```
scripts: quantum-script .js, magnet, embedding hosts, ...
quantum-script--base16    <-- this extension: Base16.encode / decode / decodeToBuffer
quantum-script--buffer    (the Buffer type returned by decodeToBuffer, loaded automatically)
quantum-script            (Executive, Variable, Context)
xyo-encoding              (XYO::Encoding::Base16::encode / decode, the codec)
xyo-system, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What `Base16` gives |
|------|---------------------|
| Show or store a binary value as text (digest, key, id) | `Base16.encode(bytes)` — 2 characters per byte, only `0-9 A-F` |
| Read hex produced by other tools (`sha256sum`, OpenSSL, databases) | `Base16.decode(hex)`, upper or lower case |
| Validate hex input | `decode` returns `undefined` on any invalid input |
| Get bytes for binary APIs | `Base16.decodeToBuffer(hex)` returns a `Buffer` |
| Same API as the other codecs | `Base32` and `Base64` have the same three functions |

Base16 is the most readable and the largest of the three: the output is
twice the input (Base64: ~1.33x, Base32: ~1.6x), but it is case-insensitive
on input, has no padding and can be checked by eye.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("Base16");` | also loads `Buffer` |
| Encode a string | `Base16.encode("foo")` | `"666F6F"` |
| Encode a buffer | `Base16.encode(buffer)` | first `length` bytes |
| Decode to a string | `Base16.decode("666f6f")` | `"foo"`; `undefined` if invalid |
| Decode to a buffer | `Base16.decodeToBuffer("00FF")` | `Buffer`, `length` 2; `undefined` if invalid |
| Check for failure | `Script.isUndefined(r)` | not `!r`: decoding `""` gives `""`, which is falsy |
| Lowercase hex | `Buffer.fromString(s).toHex()` | the `Buffer` extension writes lowercase |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, fabricare scripts, register it in a C++ host |
| [Script API](script-api.md) | Every function: argument conversion, exact behavior, edge cases, recipes |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, the DLL entry point, using the codec directly from C++ |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`; the `Buffer` type in the `quantum-script--buffer` repository,
`docs/`; the codec in the `xyo-encoding` repository, `docs/`.

## Source map

```
source/XYO/QuantumScript.Extension/Base16.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/Base16.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/Base16/
    Dependency.hpp                                       <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                   initExecutive, registerInternalExtension,
                                                         encode / decode / decodeToBuffer
    Copyright / License / Version                        library metadata
test/test.01.cpp                                         C++ host registering Console, Buffer, Base16 as internal
test/test.01.js                                          RFC 4648 test vectors, encode and decode
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--base16/`](../.claude/skills/quantum-script--base16/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use `Base16`
(Quantum Script tools, magnet scripts, other extensions).
