# String Module — How It Works

This document explains the internal mechanisms behind the GONTI string functions.

---

## 1. Overview

The string module is a minimal wrapper around standard C string functions with added integration into the engine's memory subsystem.

It provides:

* String duplication
* String length calculation
* String comparison

All allocations made here use the memory tagging system for tracking.

---

## 2. gontiStringDuplicate()

```c
char* gontiStringDuplicate(const char* str)
```

### How it works:

1. Calls `gontiStringLength(str)` to get the number of characters.
2. Allocates `length + 1` bytes using `k_allocate()` with the tag `GONTI_MEMORY_TAG_STRING`.
3. Copies the string and the null terminator using `k_copyMemory()`.

This integrates strings seamlessly into the memory tracking system.

---

## 3. gontiStringLength()

```c
u64 gontiStringLength(const char* str)
```

A simple wrapper around `strlen()`. This allows the engine to avoid including `<string.h>` everywhere and keeps the API consistent.

---

## 4. gontiStringEqual()

```c
b8 gontiStringEqual(const char* str1, const char* str2)
```

Uses `strcmp()` internally:

* Returns `true` when both strings are identical.
* Returns `false` otherwise.

This function simplifies comparisons by returning a boolean instead of integer values from `strcmp()`.

---

## 5. Memory Integration

The only function using memory allocation is `gontiStringDuplicate()`.

* Memory is allocated via `k_allocate()`.
* Memory must be freed using `k_free()`.
* Tagging ensures string memory usage appears correctly in `k_getMemoryUsageStr()`.

The remaining functions do not modify memory; they operate on existing buffers.

---

## Summary

The string module is intentionally lightweight. Its core responsibility is to provide simple, engine‑wide string utilities backed by the memory subsystem for consistent allocation tracking.
