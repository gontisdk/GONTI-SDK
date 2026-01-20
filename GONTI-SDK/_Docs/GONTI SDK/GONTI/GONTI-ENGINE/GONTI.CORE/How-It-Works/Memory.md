# Memory Subsystem — How It Works

This document explains the internal mechanisms behind the memory subsystem in `Memory.c`.

---

## 1. Purpose of the Memory Subsystem

The goal of this system is to:

* Provide tagged allocations for tracking memory grouped by subsystem.
* Centralize memory operations behind a unified API.
* Offer detailed memory usage statistics.
* Keep track of total and per‑tag allocated bytes.

The system is lightweight and does not replace an allocator — instead, it wraps platform allocations with a header.

---

## 2. Allocation Header

Every allocation reserves additional space to store metadata:

```
[GontiAllocationHeader][ Actual user memory ]
```

```c
typedef struct GontiAllocationHeader {
    GtU64 size;
    GtMemTag tag;
} GontiAllocationHeader;
```

When memory is returned to the user, it is offset by one header:

```c
void* block = (void*)(header + 1);
```

When freeing or reallocating, the block pointer is shifted back to access the header.

---

## 3. Tracking Memory Stats

The subsystem maintains global statistics:

```c
struct GontiMemoryStats {
    GtU64 totalAllocated;
    GtU64 taggedAllocations[GT_MEM_TAG_MAX_TAGS];
};
```

* `totalAllocated` counts active allocations.
* `taggedAllocations[]` stores total bytes allocated per tag.

During allocation:

* Increase total count
* Increase the tag's allocation sum

During free:

* Decrease total count
* Subtract the size stored in the header

During reallocate:

* Subtract old size, add new size

---

## 4. Allocation Process

`gt_allocate()` performs these steps:

1. Warn if tag is `UNKNOWN`
2. Increase allocation counters
3. Allocate `size + header`
4. Write header metadata
5. Zero‑initialize the user block
6. Return pointer to memory after the header

---

## 5. Freeing Memory

`gt_free()`:

1. Reads the header preceding the block
2. Updates stats accordingly
3. Calls `gontiPlatformFree()` using the header pointer

The user must always pass the pointer returned by `gt_allocate()`.

---

## 6. Reallocating Memory

`gt_reallocate()`:

1. Reads old header
2. Adjusts tag statistics
3. Calls `gontiPlatformReallocate()` on the header pointer
4. Updates header with new size
5. Returns memory pointer after the updated header

---

## 7. Memory Statistics String

`gontiGetMemoryUsageStr()` builds a formatted string containing:

* All memory tags
* Allocated size per tag
* Automatically chosen units (B, KiB, MiB, GiB)

The string is dynamically allocated through `gontiStringDuplicate()` and must be manually freed.

---

## 8. Initialization and Shutdown

`gontiInitializeMemory()` zeroes the global statistics.

`gontiShutdownMemory()` currently calls:

```c
gontiPlatformFree(&gontiMemoryStats, GtFalse);
```

This frees the stats structure itself (likely not intended), but included here as‑is based on the provided implementation.

---

## Summary

The memory subsystem is a lightweight, tag‑based tracking layer built on top of platform allocation functions. It adds no complex memory management but provides vital runtime insight into memory usage across engine subsystems.
