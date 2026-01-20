# GONTI.CORE – GtDefines.h Usage

This document describes how to use the definitions, types, macros, and utilities provided by **GtDefines.h** from the GONTI.CORE module.

## Overview

`GtDefines.h` provides a foundational layer of consistent type definitions, platform detection, boolean handling, compiler attributes, numeric limits and helper macros. It is meant to be included in all low‑level modules of the engine to ensure unified behavior across compilers and platforms.

---

## 1. Including the Header

To use any of the types or macros, simply include the file:

```c
#include "GtDefines.h"
```

This header is fully C/C++ compatible through `extern "C"` guards.

---

## 2. Using Provided Types

### Integer Types

The header defines explicit‑size integer types:

```c
GtU8, GtU16, GtU32, GtU64   // Unsigned
GtI8, GtI16, GtI32, GtI64   // Signed
```

Usage example:

```c
GtU32 counter = 0;
GtI64 position = -15;
```

### Floating‑Point Types

```c
GtF32 // 32-bit float
GtF64 // 64-bit float
```

### Boolean Types

```c
GtB8  // For C (1‑byte)
GtB8    // 8‑bit boolean
GtB32   // 32‑bit boolean
```

### Range Types

Two structures describe byte ranges:

```c
GtRange r = { .offset = 0, .size = 128 };
GtRange32 r32 = { .offset = 4, .size = 16 };
```

---

## 3. Platform Macros

The header detects the target OS and defines one of the following:

* `GTPLATFORM_WINDOWS`
* `GTPLATFORM_LINUX`
* `GTPLATFORM_ANDROID`
* `GTPLATFORM_UNIX`
* `GTPLATFORM_POSIX`
* `GTPLATFORM_APPLE`
* `GTPLATFORM_IOS`, `GTPLATFORM_IOS_SIMULATOR`

Usage:

```c
#if GTPLATFORM_LINUX
    // Linux-specific code
#endif
```

---

## 4. Import/Export Macros

Use `GTAPI` to mark exported/imported symbols when building shared libraries.

```c
GTAPI void my_function();
```

This resolves to:

* `__declspec(dllexport)` / `__declspec(dllimport)` on MSVC
* `__attribute__((visibility("default")))` on GCC/Clang

---

## 5. Build Configuration Flags

The header exposes build‑type macros:

```c
GT_DEBUG   // Enabled in debug
GT_RELEASE // Enabled in release
```

Usage:

```c
#if GT_DEBUG
    printf("Debug mode enabled\n");
#endif
```

---

## 6. Helper Macros

### Clamping Values

```c
GtU32 result = GTCLAMP(value, 0, 100);
```

### Flag Manipulation

```c
GT_FLAG_GET(flags, SOME_FLAG);
GT_FLAG_SET(flags, SOME_FLAG, GtTrue);
```

### Size Conversions

Convert units to bytes:

```c
GtU64 buffer = GT_MEBIBYTES(4); // 4 * 1024 * 1024
```

Available forms:

* `GT_GIBIBYTES`, `GT_MEBIBYTES`, `GT_KIBIBYTES`
* `GT_GIGABYTES`, `GT_MEGABYTES`, `GT_KILOBYTES`

---

## 7. Alignment Helpers

Align a value:

```c
GtU64 aligned = gontiGetAligned(130, 64);  // returns next 64-byte boundary
```

Get an aligned range:

```c
GtRange r = gontiGetAlignedRange(offset, size, 64);
```

---

## 8. Static Assertions

Use `GT_STATIC_ASSERT` when adding new type definitions.

```c
GT_STATIC_ASSERT(sizeof(GtU32) == 4, "GtU32 must be 4 bytes");
```

---

## Summary

`GtDefines.h` is the core foundational header of GONTI.CORE. Include it wherever you need:

* consistent cross‑platform types
* compiler‑agnostic API export/import
* flag and alignment utilities
* byte‑size conversions

It ensures consistency and portability across the entire engine.
