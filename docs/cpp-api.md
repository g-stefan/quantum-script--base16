# C++ API

For hosts that embed Quantum Script and for maintainers of the extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first: native functions, `Variable` and
`TPointer` work the same way here.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/Base16.hpp>   // Library.hpp

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::Base16`. Export macro:
`XYO_QUANTUMSCRIPT_EXTENSION_BASE16_EXPORT`:

| Define | Effect |
|--------|--------|
| `XYO_QUANTUMSCRIPT_EXTENSION_BASE16_INTERNAL` (or `QUANTUM_SCRIPT__BASE16_INTERNAL`, set by fabricare while building the DLL) | export macro = `XYO_PLATFORM_LIBRARY_EXPORT` |
| none | export macro = `XYO_PLATFORM_LIBRARY_IMPORT` (consumers of the DLL) |
| `XYO_QUANTUMSCRIPT_EXTENSION_BASE16_LIBRARY` | export macro empty, no `quantumScriptExtension` entry point (sources compiled into another library) |
| `XYO_PLATFORM_COMPILE_STATIC` (static platforms) | `XYO_PLATFORM_LIBRARY_EXPORT` / `IMPORT` are empty |

The `quantumScriptExtension` entry point is compiled only when
`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is defined and
`XYO_QUANTUMSCRIPT_EXTENSION_BASE16_LIBRARY` is not.

## Registering the extension

```cpp
void Extension::Base16::registerInternalExtension(Executive *executive);
void Extension::Base16::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"Base16"` as an internal extension;
  call it from the host's init callback, together with
  `Extension::Buffer::registerInternalExtension` (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `Base16` in a thread. It sets the extension name,
  info (license text), version and public flag, then:

  ```cpp
  executive->compileStringX("Script.requireExtension(\"Buffer\");");
  executive->compileStringX("var Base16={};");
  executive->setFunction2("Base16.encode(str)", encode);
  executive->setFunction2("Base16.decode(str)", decode);
  executive->setFunction2("Base16.decodeToBuffer(str)", decodeToBuffer);
  ```

  Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--base16.dll`.

## The native functions

All three are `static` in `Base16/Library.cpp` and wrap the codec in
`xyo-encoding`:

| Script function | C++ | Returns |
|-----------------|-----|---------|
| `Base16.encode(str)` | `XYO::Encoding::Base16::encode(arguments->index(0)->toString())` | `VariableString` |
| `Base16.decode(str)` | `XYO::Encoding::Base16::decode(text, result)` | `VariableString`, or `Context::getValueUndefined()` on failure |
| `Base16.decodeToBuffer(str)` | same decode | `Extension::Buffer::VariableBuffer::newVariableFromString(result)`, or undefined |

With `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` defined each function prints a trace
line (`- base16-encode`, ...).

## Using the codec without the script engine

C++ code that only needs hex conversion should call `xyo-encoding` directly
instead of going through a script:

```cpp
#include <XYO/Encoding.hpp>

XYO::Encoding::String hex = XYO::Encoding::Base16::encode("foo");   // "666F6F"

XYO::Encoding::String bytes;
if (XYO::Encoding::Base16::decode(hex, bytes)) {
	// bytes == "foo"
};
```

| Function | Behavior |
|----------|----------|
| `String encode(const String &toEncode)` | uppercase, 2 characters per byte |
| `bool decode(const String &toDecode, String &out)` | `false` (and `out` unchanged) for odd length or a non hex character; case-insensitive |

## Notes for maintainers

- New functions: register them in `initExecutive` with
  `executive->setFunction2("Base16.name(args)", name)`, then update
  `README.md`, `docs/script-api.md`, `docs/reference.md`, the skill in
  `.claude/skills/quantum-script--base16/` and `test/test.01.js`.
- Keep the API in step with `quantum-script--base32` and
  `quantum-script--base64`, which expose the same three functions.
- `test/test.01.js` checks the RFC 4648 test vectors (`""`, `"f"`, ...,
  `"foobar"`) in both directions; run it with `fabricare test`.
