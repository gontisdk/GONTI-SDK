# GONTI.CORE – Platform Module Usage

This document describes how to use the functions exposed by `Platform.h` across supported systems (Windows & Linux).

---

## 1. Overview

The Platform module provides a unified abstraction over OS-specific functionality:

* Console output (with color)
* Memory allocation utilities
* Sleep/timing utilities
* High-precision timing

All platform differences are handled inside the corresponding `.c` file. You only include `Platform.h`.

```c
#include <GONTI/Platform/Platform.h>
```

All functions are available on every platform unless marked otherwise.

---

## 2. Console Output

### Write to standard output

```c
gontiPlatformConsoleWrite("Hello World!", 3); // INFO colour example
```

### Write to standard error

```c
gontiPlatformConsoleWriteError("Something went wrong!", 1); // ERROR colour
```

Colour index mapping (universal across platforms):

| Index | Meaning |
| ----- | ------- |
| 0     | FATAL   |
| 1     | ERROR   |
| 2     | WARN    |
| 3     | INFO    |
| 4     | DEBUG   |
| 5     | TRACE   |

---

## 3. Memory Utilities

### Allocate memory

```c
void* ptr = gontiPlatformAllocate(256, false);
```

### Reallocate memory

```c
ptr = gontiPlatformReallocate(ptr, 512);
```

### Free memory

```c
gontiPlatformFree(ptr, false);
```

### Set memory

```c
gontiPlatformSetMemory(ptr, 0xFF, 128);
```

### Zero memory

```c
gontiPlatformZeroMemory(ptr, 128);
```

### Copy memory

```c
gontiPlatformCopyMemory(dest, src, 64);
```

All memory functions behave like `malloc/realloc/free/memset/memcpy`.

---

## 4. Sleeping

```c
gontiPlatformSleep(1000); // Sleep for 1 second
```

Uses `Sleep()` on Windows and either `nanosleep()` or `usleep()` on Linux.

---

## 5. High Precision Timing

### Absolute Time

Always available:

```c
f64 now = gontiPlatformGetAbsoluteTime();
```

Returns monotonic time in seconds.

### Windows-specific Clock Frequency

Only available on Windows:

```c
gontiPlatformSetClockFrequency(1.0 / frequency);
f64 freq = gontiPlatformGetClockFrequency();
```

Required internally for high-precision time calculations.

---

## Summary

You use the Platform module to:

* print colored logs
* handle memory in a platform-independent way
* sleep threads
* read high-precision time

The API is intentionally simple and mirrors C standard library functions while adding platform consistency.
