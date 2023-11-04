# Memory management

Melon provides some wrapper memory management functions, with nicer names than the originals as well as better defined behaviour.

You can include this module as `melon/alloc.h`.

## Allocating memory

Use `DgMemoryAllocate`:

```c
void *block = DgMemoryAllocate(size_in_bytes);
```

## Freeing memory

Use `DgMemoryFree`:

```c
DgMemoryFree(block);
```

Note that if `block` is `NULL` then no action is taken.

## Resize memory block

Use `DgMemoryReallocate`:

```c
void *new_block = DgMemoryReallocate(block, new_size);
```

Note that if:

* `size` is `0`, then `DgMemoryFree` is called on the block
* `size` is nonzero and `block` is `NULL`, then `DgMemoryAllocate` is called with the size

This can be used to easily create a resizeable array using only `DgMemoryReallocate`.

## Old variants

`DgAlloc`, `DgFree` and `DgRealloc` are older functions and might be removed one day. If you want to use them, look how to use them in the source.
