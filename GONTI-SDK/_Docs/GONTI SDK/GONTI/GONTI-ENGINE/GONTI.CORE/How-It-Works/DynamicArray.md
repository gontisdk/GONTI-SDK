# DynamicArray Module - How It Works

This document explains the internal mechanics of the GONTI Dynamic Array.

## Memory Layout

Each array is a memory block composed of:

1. **Header** (3 × `GtU64` fields):

   * `GT_DARRAY_CAPACITY`
   * `GT_DARRAY_LENGTH`
   * `GT_DARRAY_STRIDE` (sizeof element)
2. **Data section** containing raw element bytes.

```
| capacity | length | stride | element0 | element1 | ... |
```

The pointer returned to the user points directly to the **data section**, not the header.

## Creating an Array

`_gontiDarrayCreate()`:

* Allocates memory for header + data.
* Zero-initializes the block.
* Writes capacity, length=0, stride.
* Returns pointer to the first element (header offset by `GT_DARRAY_FIELD_LENGTH`).

## Header Access

`_gontiDarrayFieldGet()` and `_gontiDarrayFieldSet()` compute the header location by subtracting the header length from the user pointer:

```c
GtU64* header = (GtU64*)array - GT_DARRAY_FIELD_LENGTH;
```

## Resizing

Triggered when `length >= capacity`.
`_gontiDarrayResize()`:

1. Creates a new array with capacity * 2.
2. Copies element bytes (`length * stride`).
3. Transfers length.
4. Frees old array.
5. Returns the new array pointer.

Since reallocation may move memory, **the returned pointer must replace the original**.

## Push Operation

* Gets current length and stride.
* Resizes if needed.
* Computes destination address:

```c
addr = (GtU64)array + length * stride;
```

* Copies new element into this address.
* Increments length.

## Pop Operation

Reads last element, copies it to `dest`, then decrements length.

## InsertAt

If inserting in the middle:

* Shifts all elements right using `gt_copyMemory`.
* Writes new value.
* Increments length.

## PopAt

If index is valid:

* Copies removed element to `dest`.
* Shifts elements left to fill gap.
* Decrements length.

## Memory Management

* Allocations use GONTI’s custom subsystem (`gt_allocate`, `gt_free`).
* Tagged with `GT_MEM_TAG_DARRAY` for statistics.

The result is a flexible, generic, C‑macro‑driven dynamic array that behaves similarly to `std::vector` but remains fully type‑agnostic and manual-memory-friendly.
