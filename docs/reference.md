# API reference

## Script

Available after `Script.requireExtension("Base16")` (which also loads
`Buffer`).

| Symbol | Returns | Notes |
|--------|---------|-------|
| `Base16.encode(str)` | String | each byte of `toString(str)` as 2 uppercase hex digits; never fails |
| `Base16.decode(str)` | String or `undefined` | even length, only `0-9 a-f A-F`; otherwise `undefined` |
| `Base16.decodeToBuffer(str)` | Buffer or `undefined` | same validation as `decode`; `size == length` = decoded bytes |

### Edge cases

| Expression | Result |
|------------|--------|
| `Base16.encode("")` | `""` |
| `Base16.encode(buffer)` | hex of the first `length` bytes |
| `Base16.encode(255)` | `"323535"` (the text `"255"`) |
| `Base16.encode()` | `"756E646566696E6564"` (the text `"undefined"`) |
| `Base16.decode("")` | `""` |
| `Base16.decode("abc")` | `undefined` (odd length) |
| `Base16.decode("0x41")`, `"41 42"`, `"GG"` | `undefined` |
| `Base16.decode("610062").length` | `3` (zero byte kept) |
| `Base16.decodeToBuffer("")` | empty Buffer (`length` 0, falsy) |

### Errors

| Message | Cause |
|---------|-------|
| `Unable to open "Base16"` | the extension library was not found and no internal one is registered (for example in `fabricare` scripts) |

The three functions themselves never throw: invalid input to `decode` /
`decodeToBuffer` gives `undefined`.

## C++

Namespace `XYO::QuantumScript::Extension::Base16`, umbrella header
`<XYO/QuantumScript.Extension/Base16.hpp>`.

### Library (`Base16/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"Base16"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (not in static builds) |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::Base16::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--base16` | fabricare project, `dll-or-lib` |
| `test.01` | fabricare test project, runs `test/test.01.js` |
| `XYO_QUANTUMSCRIPT_EXTENSION_BASE16_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_BASE16_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__BASE16_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_BASE16_LIBRARY` | static build: empty export macro, no DLL entry point |

### Codec (`xyo-encoding`)

| Symbol | Notes |
|--------|-------|
| `XYO::Encoding::Base16::encode(const String &)` | uppercase hex |
| `XYO::Encoding::Base16::decode(const String &, String &out)` | `bool`; strict, case-insensitive |
