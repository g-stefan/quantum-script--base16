# Quantum Script Extension Base16

Quantum Script extension
- Hexadecimal (Base16, RFC 4648) encoding of strings and buffers:
`Base16.encode` writes every byte as two uppercase hex digits.
- Strict decoding back to a `String` or a `Buffer`: `Base16.decode`,
`Base16.decodeToBuffer` return `undefined` on invalid input.
- For putting binary data (digests, keys, tokens) into text: logs, JSON,
configuration files, command lines.

```javascript
Script.requireExtension("Base16");

Base16;
Base16.encode(str);
Base16.decode(str);
Base16.decodeToBuffer(str);
```

Built on `quantum-script` and `quantum-script--buffer`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, load from a script, hosts, register in a C++ host
- [Script API](docs/script-api.md) - every function: exact behavior, edge cases, recipes
- [C++ API](docs/cpp-api.md) - registration, DLL entry point, the codec in `xyo-encoding`
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--base16](.claude/skills/quantum-script--base16/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
