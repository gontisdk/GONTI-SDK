# DynamicArrayEx – Usage

## Creating arrays

```c
GtDArrayEx arr = gontiDarrayExDefaultAllocate();
GtDArrayEx arr2 = gontiDarrayExAllocate(16);
```

## Adding elements

```c
gontiDarrayExAddLast(&arr, somePtr);
gontiDarrayExAddFirst(&arr, otherPtr);
gontiDarrayExAddAtIdx(&arr, ptr, 3);
```

## Getting elements

```c
void* el = gontiDarrayExGet(&arr, 2);
```

## Removing elements

```c
void* removed = gontiDarrayExRemoveAtIdx(&arr, 1);
removed = gontiDarrayExRemoveFirst(&arr);
removed = gontiDarrayExRemoveLast(&arr);
```

## Iterating

```c
void visitor(void* e) {
    // ...
}
gontiDarrayExIterate(&arr, visitor);
```

### Iterator-based

```c
GtDArrayExIterator it = gontiDarrayExIteratorNew(&arr);
void* e;
while ((e = gontiDarrayExIteratorNext(&it))) {
    // ...
}
```

## Clearing and freeing

```c
gontiDarrayExClear(&arr);
gontiDarrayExFree(&arr);
gontiDarrayExFreeDeep(&arr);
```
