## 0.0.9 (2024-09-13)

- support for NEON instructions in AARCH64
- handling some new VEX encoded instructions (x86)
- more systematic branchless semantics
  (e.g. [#37](https://github.com/binsec/binsec/issues/37))
- various bug fixes and code improvements

## 0.0.8 (2024-03-08)

- support for `adcx`, `adox` and `mulx` (x86)
- some bug fixes and code improvements

## 0.0.7 (2023-10-23)

- various bug fixes and code improvements

## 0.0.6 (2023-07-14)

- add PowerPC 64 bit decoder
- various bug fixes and code improvements

## 0.0.5 (2023-02-24)

- enable macOS Homebrew distribution

## 0.0.4 (2023-02-15)

- better handling of x86 flags
- various bug fixes and refactoring

## 0.0.3 (2022-07-24)

- handling some new VEX encoded instructions (x86)
- various bug fixes and refactoring

## 0.0.2 (2022-04-15)

- handling of SSE instructions for AMD64
- various bug fixes and code generator improvements

## 0.0.1 (2022-03-14)

- update build system (dune 3.0) and improve dependency tracking
- fix memory leak issues
- various bug fixes and code improvements

## 0.0.0 (2021-10-05)

Initial release.

OCaml binding for the UNISIM DBA (Dynamic Bitvector Automata) decoder.
- `Arm32dba` library exposes ARM/Thumb architecture;
- `Aarch64dba` library exposes AARCH64 architecture;
- `Amd64dba` library exposes AMD64 architecture.
