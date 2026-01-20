# DynamicArray Module - Usage

This module provides a generic, type-agnostic dynamic array implementation using macros and an internal header.

## Creating a Dynamic Array

Create a new dynamic array for any type:

```c
GtB32* arr = gontiDarrayCreate(GtB32);
```

Or reserve a custom initial capacity:

```c
GtB32* arr = gontiDarrayReserve(GtB32, 32);
```

## Destroying the Array

Always destroy the array when done:

```c
gontiDarrayDestroy(arr);
```

## Adding Elements

Push values to the end:

```c
gontiDarrayPush(arr, 10);
gontiDarrayPush(arr, 20);
```

Insert at any index:

```c
gontiDarrayInsertAt(arr, 1, 99);
```

## Removing Elements

Pop last element:

```c
GtB32 value;
gontiDarrayPop(arr, &value);
```

Pop at index:

```c
GtB32 removed;
arr = gontiDarrayPopAt(arr, 0, &removed);
```

## Array Properties

```c
GtU64 cap = gontiDarrayCapacity(arr);
GtU64 len = gontiDarrayLength(arr);
GtU64 stride = gontiDarrayStride(arr);
```

## Clearing

```c
gontiDarrayClear(arr);
```

## Notes

* The array pointer may change after push/insert due to resizing.
* You must reassign the array after operations that may return a new pointer:

```c
arr = gontiDarrayPush(arr, x);
```
