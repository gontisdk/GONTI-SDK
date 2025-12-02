# Memory Subsystem — Usage

This document describes how to use the memory subsystem provided by `Memory.h` and `Memory.c`. All examples and explanations assume that GONTI.CORE is already initialized and that platform functions are available.

---

## Initialization

Before using any memory functions, initialize the subsystem:

```c
gontiInitializeMemory();
```

To clean up on shutdown:

```c
gontiShutdownMemory();
```

---

## Allocating Memory

Use `k_allocate(size, tag)` to allocate zero‑initialized memory with a specific tag:

```c
MyStruct* data = (MyStruct*) k_allocate(sizeof(MyStruct), GONTI_MEMORY_TAG_APPLICATION);
```

The memory will automatically be tracked under the provided tag.

---

## Freeing Memory

Free any block previously allocated with `k_free()`:

```c
k_free(data);
```

The system automatically updates internal statistics based on the block's tag and allocation header.

---

## Reallocating Memory

Resize an existing block:

```c
data = (MyStruct*) k_reallocate(data, sizeof(MyStruct) * 2);
```

Reallocation adjusts memory statistics and preserves the tag from the original allocation.

---

## Memory Operations

The following utility functions proxy platform‑level memory operations:

```c
k_zeroMemory(block, size);
k_copyMemory(dest, source, size);
k_setMemory(dest, value, size);
```

These work identically to standard `memset`, `memcpy`, etc.

---

## Getting Memory Statistics

Retrieve a dynamically allocated string describing usage by memory tag:

```c
char* memStr = k_getMemoryUsageStr();
printf("%s", memStr);
k_free(memStr);  // Remember to free it
```

The returned string contains a human‑readable breakdown of memory usage for each subsystem tag.
