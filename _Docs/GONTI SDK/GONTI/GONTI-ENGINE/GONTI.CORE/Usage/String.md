# String Module — Usage

This document explains how to use the string utilities provided in `String.h` and `String.c`.

---

## String Duplication

Creates a heap-allocated copy of a null-terminated string:

```c
char* copy = gontiStringDuplicate("Hello World");
```

The memory is allocated with the `GONTI_MEMORY_TAG_STRING` tag and must be freed using:

```c
k_free(copy);
```

---

## String Length

Retrieves the length of a C-string (excluding null terminator):

```c
u64 len = gontiStringLength("Example");
```

This is a direct wrapper around `strlen()`.

---

## String Equality

Compares two null-terminated strings:

```c
if (gontiStringEqual(str1, str2)) {
    // Strings are identical
}
```

Returns `true` if both strings match exactly.
