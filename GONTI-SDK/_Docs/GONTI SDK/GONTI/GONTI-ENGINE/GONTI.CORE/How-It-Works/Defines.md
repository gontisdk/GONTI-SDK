# GONTI.CORE – GtDefines.h How It Works

This document explains the internal logic, design reasoning, and mechanisms behind the `GtDefines.h` header in GONTI.CORE.

---

## 1. Purpose of the File

`GtDefines.h` serves as the lowest-level foundational header for the entire GONTI engine. Its goals are:

* ensure consistent data types across all platforms and compilers
* provide predictable behavior on C and C++
* offer compile-time safety mechanisms (static assertions)
* define platform-specific macros
* standardize compiler features like inlining, deprecation, and symbol visibility
* provide commonly used utilities (clamping, alignment, flags, byte conversions)

It is designed to be extremely lightweight and dependency-free.

---

## 2. C/C++ Compatibility Layer

The header begins with:

```c
#ifdef __cplusplus
extern "C" {
#endif
```

This ensures that when included in C++ sources, all definitions have **C linkage**, preventing name mangling. This guarantees compatibility with C libraries and allows GONTI modules written in C to interoperate with C++ systems.

---

## 3. Boolean Normalization in C

Since C (pre-C99) lacks a built-in `GtB8`, the file ensures a predictable definition:

```c
typedef unsigned char GtB8;
#define GtTrue 1
#define GtFalse 0
```

This is wrapped in C-only guards so that C++ (which has `GtB8`) is unaffected.

This guarantees that every module can rely on a consistent boolean type.

---

## 4. Custom Fixed-Width Primitive Types

The header defines a complete set of integer, floating-point, and boolean types:

* `GtU8/GtU16/GtU32/GtU64`
* `GtI8/GtI16/GtI32/GtI64`
* `GtF32/GtF64`
* `GtB8/GtB32`

These types make low-level work predictable and platform-independent. For example:

* `unsigned long` is **not** guaranteed to be 64-bit on all platforms
* `int` may vary between 16, 32, or even 64 bits depending on architecture

Explicit-width typedefs eliminate ambiguity.

---

## 5. Range Structures

Two structs are included:

```c
GtRange   // 64-bit offsets
GtRange32  // 32-bit offsets
```

These are used for describing memory blocks, buffer regions, or aligned ranges. Keeping them separate avoids unnecessary overhead when 32-bit precision is sufficient.

---

## 6. Static Assertions for Type Safety

After defining the base types, the file validates their sizes:

```c
GT_STATIC_ASSERT(sizeof(GtU32) == 4, "Expected GtU32 to be 4 bytes.");
```

These compile-time assertions ensure the typedefs behave consistently across architectures.

If a compiler/platform violates an expected size, compilation fails immediately, preventing subtle undefined behavior.

---

## 7. Numeric Limits and Invalid IDs

The header defines standardized constants like:

* `GT_U64_MAX`, `GT_U32_MAX`, ...
* `GT_I64_MIN`, `GT_I32_MIN`, ...
* `GT_INVALID_ID`, `GT_INVALID_ID_U64`, etc.

These provide:

* predictable numeric bounds
* sentinel IDs used throughout the engine to identify invalid objects, handles, or resources

The unavailable/invalid values are chosen to be all-bits-set for their respective size, which aligns with common engine practices.

---

## 8. Platform Detection System

The file uses compiler-provided macros to detect the operating system:

```c
#if defined(WIN32) || defined(_WIN32)
    #define GTPLATFORM_WINDOWS 1
```

Equivalent branches exist for Linux, Android, Apple platforms, POSIX environments, and Unix-like systems.

This allows the engine to compile platform‑specific code conditionally.

In some cases, additional actions occur:

* ensuring **64-bit Windows** (`#error` if not)
* defining Vulkan-related flags on macOS (`VK_USE_PLATFORM_MACOS_MVK`)

---

## 9. Symbol Import/Export (GTAPI)

Shared libraries require special handling for symbol visibility.

The macro:

```c
GTAPI
```

expands to:

* `__declspec(dllexport)` / `__declspec(dllimport)` on Windows
* `__attribute__((visibility("default")))` on GCC/Clang

This makes modules using this header cross-compiler compatible.

---

## 10. Build Configuration

The file uses `_DEBUG` (MSVC standard) to define:

```c
GT_DEBUG
GT_RELEASE
```

These macros allow build-dependent logic anywhere in the engine.

---

## 11. Utility Macros

### Clamping

```c
#define GTCLAMP(value, min, max)
```

This uses a nested ternary operator for speed and inlining without branching overhead.

### Flag Handling

Two macros manipulate bitmasks:

```c
GT_FLAG_GET(flags, flag)
GT_FLAG_SET(flags, flag, enabled)
```

These simplify feature toggles or state tracking.

### Byte Conversion

Macros like `GT_GIBIBYTES(amount)` use only multiplication by powers of two or ten, computed at compile time.

---

## 12. Alignment Helpers

Two inline functions provide memory alignment support:

### `gontiGetAligned`

Aligns a number to the next boundary:

```c
return (operand + (granularity - 1)) & ~(granularity - 1);
```

This is a classic bitmask alignment formula:

* add `granularity - 1`
* clear the lowest bits

### `gontiGetAlignedRange`

Uses the above function to align both offset and size and wrap them in `GtRange`.

These utilities appear frequently in buffer allocation, renderer memory management and asset loading.

---

## 13. Inlining & Deprecation

Compiler‑specific attributes standardize function inlining and deprecation across platforms.

Examples:

* `__forceinline` for MSVC
* `__attribute__((always_inline))` for Clang/GCC

This levels compiler differences and allows the engine to enforce performance-critical inlining.

---

## 14. Design Principles Behind GtDefines.h

The header follows these design goals:

* **Zero-dependency**: no external headers besides platform checks.
* **Portable**: every platform is normalized.
* **Explicit**: no ambiguous or compiler-dependent types.
* **Fail-fast**: use static assertions to detect incorrect assumptions.
* **Performance-first**: macros and inline functions avoid unnecessary overhead.
* **Minimalistic**: only fundamental low-level elements are included.

---

## Summary

`GtDefines.h` is the binary and platform foundation of GONTI.CORE. It ensures that:

* types are predictable
* platforms are handled uniformly
* symbols are exported consistently
* memory operations follow strict alignment rules
* build configurations are standardized

Everything else in the engine relies on the guarantees established by this file.
