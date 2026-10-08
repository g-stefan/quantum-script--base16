# Script API

Everything the extension defines after `Script.requireExtension("Base16")`.
`Base16` is a plain object holding three native functions; it has no
constructor and no state.

Every function takes one argument and converts it to a string with the
engine's `toString` first. A Quantum Script string is a sequence of bytes
(UTF-8 by convention, zero bytes allowed), and Base16 works on those bytes.

## `Base16.encode(str)`

Returns a `String` with every byte of `str` written as two **uppercase**
hexadecimal digits, most significant digit first. The result is exactly
twice as long as the input and contains only `0-9` and `A-F`.

```javascript
Base16.encode("");          // ""
Base16.encode("f");         // "66"
Base16.encode("foobar");    // "666F6F626172"
Base16.encode("ă");         // "C483"  (UTF-8 bytes c4 83)
```

Argument conversion — anything that is not a string is encoded as its text
form, which is rarely what you want:

| Argument | Encoded text | Result |
|----------|--------------|--------|
| `Buffer` | its first `length` bytes | `Base16.encode(Buffer.fromHex("00ff"))` → `"00FF"` |
| number | decimal text | `Base16.encode(255)` → `"323535"` (not `"FF"`) |
| `true` / `null` | `"true"` / `"null"` | `"74727565"` / `"6E756C6C"` |
| missing / `undefined` | `"undefined"` | `"756E646566696E6564"` — no error |
| array | elements joined with `,` | `Base16.encode([1, 2])` → `"312C32"` |

To encode the value of a number as a byte, put the byte into a buffer:
`var b = Buffer(1); b.setU8(0, 255); Base16.encode(b);` → `"FF"`.

The `Buffer` extension's `toHex()` gives the same digits in **lowercase**:
`Buffer.fromString("Hello").toHex()` is `"48656c6c6f"`, `Base16.encode("Hello")`
is `"48656C6C6F"`. Compare hex strings case-insensitively, or normalize them
with `toUpperCaseASCII()` / `toLowerCaseASCII()` (Quantum Script has no
`toUpperCase`).

## `Base16.decode(str)`

Decodes hexadecimal text and returns the bytes as a `String`, or
**`undefined`** when the text is not valid Base16.

Valid input:

- an **even** number of characters;
- every character is `0-9`, `a-f` or `A-F` (cases may be mixed);
- nothing else — no spaces, line breaks, `0x` prefixes, `:` or `-`
  separators.

```javascript
Base16.decode("666F6F");      // "foo"
Base16.decode("666f6F");      // "foo"
Base16.decode("");            // ""  (valid, empty)
Base16.decode("ABC");         // undefined: odd length
Base16.decode("ZZ");          // undefined: not a hex digit
Base16.decode("0x41");        // undefined: "x"
Base16.decode("41 42");       // undefined: space
Base16.decode();              // undefined: "undefined" has 9 characters
Base16.decode(4142);          // "AB": the number is converted to the text "4142"
```

- Decoding is all or nothing: there is no partial result and no error
  message, only `undefined`.
- Zero bytes are kept: `Base16.decode("610062").length` is `3`.
- The result is not checked for UTF-8: decoding a digest gives a string of
  raw bytes. Use `decodeToBuffer` when the data is binary.
- **Test for failure with `Script.isUndefined(r)`** (or `r === undefined`),
  not with `!r`: a valid empty input decodes to `""`, which is falsy too.

To accept looser input, clean it first:

```javascript
function decodeLoose(text) {
	text = text.replace(" ", "").replace(":", "").replace("\r", "").replace("\n", "");
	if (text.indexOf("0x") == 0 || text.indexOf("0X") == 0) {
		text = text.substring(2);
	};
	return Base16.decode(text);
};
```

## `Base16.decodeToBuffer(str)`

Same validation as `decode`, but returns the bytes as a **`Buffer`** (from
the `Buffer` extension), or `undefined` when the text is not valid Base16.

```javascript
var b = Base16.decodeToBuffer("00FF10");
typeof(b);       // "Buffer"
b.length;        // 3
b.size;          // 3
b.getU8(1);      // 255
b.toHex();       // "00ff10"

Base16.decodeToBuffer("XY");   // undefined
Base16.decodeToBuffer("");     // empty Buffer, length 0
```

- The buffer is new: `size == length ==` number of decoded bytes.
- An empty buffer is falsy (`if (b)` tests `length > 0`): use
  `Script.isUndefined(b)` to detect invalid input here too.
- Use it to feed binary APIs: `File.writeFromBuffer`, `Socket.writeFromBuffer`,
  `Shell.filePutContentsBuffer`, `Crypt.decrypt`, `OpenSSL` functions.

## Recipes

### Hex digest of a file

```javascript
Script.requireExtension("Shell");
Script.requireExtension("SHA256");
Script.requireExtension("Base16");

var data = Shell.fileGetContentsBuffer("setup.exe");
Base16.encode(SHA256.hashToBuffer(data));   // 64 uppercase digits
// SHA256.hash(data) gives the same digits in lowercase
```

### Store binary data in JSON

`JSON.encode` turns a `Buffer` into `null`; store its hex form instead.

```javascript
Script.requireExtension("JSON");
Script.requireExtension("Base16");

var key = Base16.decodeToBuffer("8F1C00A2");
var text = JSON.encode({key: Base16.encode(key)});   // {"key":"8F1C00A2"}
var back = Base16.decodeToBuffer(JSON.decode(text).key);
```

### Validate user input

```javascript
function parseHexKey(text, bytes) {
	var key = Base16.decodeToBuffer(text);
	if (Script.isUndefined(key) || key.length != bytes) {
		throw "expected " + (bytes * 2) + " hex digits";
	};
	return key;
};

var key = parseHexKey("00112233445566778899AABBCCDDEEFF", 16);
```

### Write a binary file from hex

```javascript
Script.requireExtension("Shell");
Script.requireExtension("Base16");

Shell.filePutContentsBuffer("magic.bin", Base16.decodeToBuffer("89504E470D0A1A0A"));
```

### Round trip

For any string `s`: `Base16.decode(Base16.encode(s)) == s`. For any valid
hex `h`: `Base16.encode(Base16.decode(h)) == h.toUpperCaseASCII()`.

## Related extensions

| Extension | Same three functions | Output alphabet |
|-----------|----------------------|-----------------|
| `Base16` | `encode`, `decode`, `decodeToBuffer` | `0-9 A-F` |
| `Base32` | `encode`, `decode`, `decodeToBuffer` | `A-Z 2-7`, `=` padding |
| `Base64` | `encode`, `decode`, `decodeToBuffer` | `A-Z a-z 0-9 + /`, `=` padding |
| `Buffer` | `Buffer.fromHex(str)`, `b.toHex()` | lowercase hex, lenient decoding (invalid digit → 0) |

`Buffer.fromHex` never fails: it treats invalid digits as `0` and ignores an
odd last character. Use `Base16.decodeToBuffer` when the input must be
validated.
