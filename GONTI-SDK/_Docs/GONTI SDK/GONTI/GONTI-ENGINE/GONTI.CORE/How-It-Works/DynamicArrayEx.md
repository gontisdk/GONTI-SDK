# DynamicArrayEx – How It Works

## Structure

The dynamic array stores:

* **size** – number of used slots
* **capacity** – number of allocated slots
* **list** – array of `void*` pointers

## Allocation

`gontiDarrayExAllocate`:

* ensures capacity is at least 1
* allocates `capacity * sizeof(void*)`
* sets `size = 0`
* on failure: `capacity = 0`

## Auto‑resizing

`gontiDarrayExReallocate`:

* computes required `newSize = size + additionalLength`
* if newSize exceeds capacity, capacity is doubled until large enough
* attempts `gt_reallocate`
* if it fails, allocates new memory and copies old block manually

## Adding elements

`gontiDarrayExAddAtIdx`:

* ensures capacity via reallocation
* shifts elements right from the end to `idx`
* places new element
* increments size

`AddFirst` and `AddLast` are simple wrappers.

## Removing elements

`gontiDarrayExRemoveAtIdx`:

* returns NULL if idx out of range
* stores removed element pointer
* shifts elements left from idx+1 to end
* sets last pointer to NULL
* decrements size

`RemoveFirst` and `RemoveLast` wrap this helper.

## Getting elements

`gontiDarrayExGet` returns pointer at index or NULL if out of range.

## Iterator

The iterator struct stores:

* pointer to the list
* current index

`IteratorNext`:

* returns next pointer if available
* increments index
* returns NULL when finished

## Clearing / Freeing

* **Clear**: frees internal buffer, resets size, resets capacity, allocates new buffer of default size.
* **Free**: frees only the list buffer.
* **FreeDeep**: frees *each element* held inside, but not the list structure itself.
