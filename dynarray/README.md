# dynarray

A robust, lightning-fast, and memory-safe Single-Header C Library for dynamic memory allocation.

`dynarray` provides a low-level, type-aware memory buffer. It is designed to be the foundational pillar for higher-level data structures (like Arraylists, Hash Tables, and Ring Buffers) while strictly adhering to system programming best practices.

## Features
* **Single-Header Library:** Drop `dynarray.h` into your project and start coding. No build systems or Makefiles required.
* **Fully Generic:** Store standard integers using the built-in size enums, or seamlessly store any complex custom struct by simply passing its `sizeof()`.
* **Dual-Mode Access:** Provides both bounds-checked accessors for safe development and unchecked accessors for high-performance production loops.
* **Zero Binary Bloat:** Heavy memory functions are compiled exactly once via the `DYNARR_IMPL` macro.
* **Graceful Failures:** Safely handles `malloc` allocation failures without crashing.

## Installation

1. Download `dynarray.h` and place it in your project folder.
2. In **exactly one** `.c` file, define the implementation macro before including the header:

```c
#define DYNARR_IMPL
#include "dynarray.h"
```

In all other files, simply include the header normally (`#include "dynarray.h"`).

## Quick Start (Standard Integers)

```c
#include <stdio.h>
#include <stdint.h>

#define DYNARR_IMPL
#include "dynarray.h"

int main(void) {
    // 1. Create initial data
    int32_t initial_data[] = {10, 20, 30, 40};

    // 2. Initialize the dynamic array
    dynarray_t list = dafrom(initial_data, 4, S32);

    // 3. Verify memory allocation succeeded
    if (!davalid(&list)) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // 4. Safely set and get values
    int32_t new_val = 99;
    daset(&list, 0, &new_val);

    int32_t *retrieved = (int32_t *)daget(&list, 0);
    if (retrieved != NULL) {
        printf("First element is: %d\n", *retrieved); // Prints 99
    }

    // 5. Grow the array
    if (darealloc(&list, 10) == 0) {
        printf("Array successfully expanded to hold 10 items.\n");
    }

    // 6. Free the memory
    dafree(&list);

    return 0;
}
```

## Advanced Usage: Custom Structs

Because `TYPE_SIZE` evaluates as a standard integer at compile time, `dynarray` is completely generic. You can use it to store and manage arrays of your own custom data structures by passing their size into the constructor.

```c
typedef struct {
    float x, y, z;
    int entity_id;
} entity_t;

// 1. Initialize a dynamic array where each element is the size of entity_t
dynarray_t entities = danew(sizeof(entity_t));

// 2. Allocate space for 10 entities
darealloc(&entities, 10);

// 3. Store a custom struct
entity_t player = {1.0f, 2.0f, 3.0f, 42};
daset(&entities, 0, &player);

// 4. Retrieve the struct
entity_t *retrieved = (entity_t *)daget(&entities, 0);
printf("Player ID: %d\n", retrieved->entity_id);

dafree(&entities);
```

## API Reference

### Core Functions
These heavy functions manage system memory and must be compiled via the `#define DYNARR_IMPL` macro.

| Function | Description |
| :--- | :--- |
| `danew(ts)` | Creates and returns a new, empty dynarray of size `ts` (can be an enum like `S32` or `sizeof(custom_struct)`). |
| `dafrom(data, n, ts)` | Allocates and copies `n` objects from `data` into a new dynarray. Returns an empty dynarray on memory failure. |
| `darealloc(da, n)` | Expands or shrinks the array to hold `n` objects. Returns `0` on success, `1` on failure. |
| `dafree(da)` | Frees the underlying memory and securely sets the pointer to `NULL`. |

### Inline Accessors
These functions are declared `static inline` for maximum performance and are available in any file that includes the header.

| Function | Description |
| :--- | :--- |
| `davalid(da)` | Returns `1` if the array is safely initialized, `0` otherwise. |
| `dasize(da)` | Returns the total physical size of the array in bytes. |
| `daobjects(da)` | Returns the maximum number of objects the array can currently hold. |
| `daget(da, index)` | Returns a pointer to the element at `index`. **Performs strict bounds checking** (returns `NULL` if out of bounds). |
| `dauget(da, index)` | Unchecked version of `daget` for high-performance loops. |
| `daset(da, index, value)`| Copies `value` into the array at `index`. **Performs strict bounds checking** (returns `1` on failure, `0` on success). |
| `dauset(da, index, value)`| Unchecked version of `daset` for high-performance loops. |

## Utility Macros
The library includes helper macros to easily pass literal values into `daset` and `dauset` by reference:
* `S8_REF(value)`
* `S16_REF(value)`
* `S32_REF(value)`
* `S64_REF(value)`

**Example:**
```c
daset(&list, 2, S32_REF(404));
```
