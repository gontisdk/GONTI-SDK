# GONTI.CORE – Defines.h Usage

This document describes how to use the definitions, types, macros, and utilities provided by **Defines.h** from the GONTI.CORE module.

## Overview

`Defines.h` provides a foundational layer of consistent type definitions, platform detection, boolean handling, compiler attributes, numeric limits and helper macros. It is meant to be included in all low‑level modules of the engine to ensure unified behavior across compilers and platforms.

---

## 1. Including the Header

To use any of the types or macros, simply include the file:

```c
#include "Defines.h"
```

This header is fully C/C++ compatible through `extern "C"` guards.

---

## 2. Using Provided Types

### Integer Types

The header defines explicit‑size integer types:

```c
u8, u16, u32, u64   // Unsigned
i8, i16, i32, i64   // Signed
```

Usage example:

```c
u32 counter = 0;
i64 position = -15;
```

### Floating‑Point Types

```c
f32 // 32-bit float
af64 // 64-bit float
```

### Boolean Types

```c
bool  // For C (1‑byte)
b8    // 8‑bit boolean
b32   // 32‑bit boolean
```

### Range Types

Two structures describe byte ranges:

```c
krange r = { .offset = 0, .size = 128 };
range32 r32 = { .offset = 4, .size = 16 };
```

---

## 3. Platform Macros

The header detects the target OS and defines one of the following:

* `KPLATFORM_WINDOWS`
* `KPLATFORM_LINUX`
* `KPLATFORM_ANDROID`
* `KPLATFORM_UNIX`
* `KPLATFORM_POSIX`
* `KPLATFORM_APPLE`
* `KPLATFORM_IOS`, `KPLATFORM_IOS_SIMULATOR`

Usage:

```c
#if KPLATFORM_LINUX
    // Linux-specific code
#endif
```

---

## 4. Import/Export Macros

Use `KAPI` to mark exported/imported symbols when building shared libraries.

```c
KAPI void my_function();
```

This resolves to:

* `__declspec(dllexport)` / `__declspec(dllimport)` on MSVC
* `__attribute__((visibility("default")))` on GCC/Clang

---

## 5. Build Configuration Flags

The header exposes build‑type macros:

```c
GONTI_DEBUG   // Enabled in debug
GONTI_RELEASE // Enabled in release
```

Usage:

```c
#if GONTI_DEBUG
    printf("Debug mode enabled\n");
#endif
```

---

## 6. Helper Macros

### Clamping Values

```c
u32 result = KCLAMP(value, 0, 100);
```

### Flag Manipulation

```c
FLAG_GET(flags, SOME_FLAG);
FLAG_SET(flags, SOME_FLAG, true);
```

### Size Conversions

Convert units to bytes:

```c
u64 buffer = MEBIBYTES(4); // 4 * 1024 * 1024
```

Available forms:

* `GIBIBYTES`, `MEBIBYTES`, `KIBIBYTES`
* `GIGABYTES`, `MEGABYTES`, `KILOBYTES`

---

## 7. Alignment Helpers

Align a value:

```c
u64 aligned = get_aligned(130, 64);  // returns next 64-byte boundary
```

Get an aligned range:

```c
krange r = get_aligned_range(offset, size, 64);
```

---

## 8. Static Assertions

Use `STATIC_ASSERT` when adding new type definitions.

```c
STATIC_ASSERT(sizeof(u32) == 4, "u32 must be 4 bytes");
```

---

## Summary

`Defines.h` is the core foundational header of GONTI.CORE. Include it wherever you need:

* consistent cross‑platform types
* compiler‑agnostic API export/import
* flag and alignment utilities
* byte‑size conversions

It ensures consistency and portability across the entire engine.
