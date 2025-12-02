# DynamicArray Module - Usage

This module provides a generic, type-agnostic dynamic array implementation using macros and an internal header.

## Creating a Dynamic Array

Create a new dynamic array for any type:

```c
int* arr = gontiDarrayCreate(int);
```

Or reserve a custom initial capacity:

```c
int* arr = gontiDarrayReserve(int, 32);
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
int value;
gontiDarrayPop(arr, &value);
```

Pop at index:

```c
int removed;
arr = gontiDarrayPopAt(arr, 0, &removed);
```

## Array Properties

```c
u64 cap = gontiDarrayCapacity(arr);
u64 len = gontiDarrayLength(arr);
u64 stride = gontiDarrayStride(arr);
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
