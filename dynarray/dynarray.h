#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

/* Utility macros */
#define S8_REF(value)  &(int8_t){ (value) }
#define S16_REF(value) &(int16_t){ (value) }
#define S32_REF(value) &(int32_t){ (value) }
#define S64_REF(value) &(int64_t){ (value) }


typedef enum {
    S8 = sizeof(int8_t),
    S16 = sizeof(int16_t),
    S32 = sizeof(int32_t),
    S64 = sizeof(int64_t),
} TYPE_SIZE;

typedef struct {
    void *ptr;
    size_t size;
    TYPE_SIZE ts;
} dynarray_t;


/* PROTOTYPES FOR SHARED FUNCTIONS */

dynarray_t danew(TYPE_SIZE ts);
dynarray_t dafrom(const void *data, const size_t n, const TYPE_SIZE ts);
uint8_t darealloc(dynarray_t *da, size_t n);
void dafree(dynarray_t *da);

/* =============================== */

#ifdef DYNARR_IMPL

/*
   This function creates a new blank dynarray of **ts** size
 */
dynarray_t danew(TYPE_SIZE ts) {
    return (dynarray_t) { .ptr = NULL, .size = 0, .ts = ts};
}

/*
   This function copies **n** objects, **ts** byte large from **data**
   inside a new dynarray 

   If the underlying malloc fails, or if **n** is 0, then a new blank
   dynarray is returned. Therefore, it is strongly recommended for the caller to 
   use `davalid` to check whether the returned dynarray is safe to access.
 */
dynarray_t dafrom(const void *data, const size_t n, const TYPE_SIZE ts) {
    // NOTE: dynarray may benefit a lot from slab allocation
    // but a slab_realloc method should be implemented

    assert(data != NULL);

    // the caller wants an empty dynarray
    if (n == 0) return danew(ts);

    const size_t size = n * ts;

    void *ptr = malloc(size);
    // allocation failed
    if (ptr == NULL) return danew(ts);

    memcpy(ptr, data, size);
    return (dynarray_t) { .ptr = ptr, .size = size , .ts = ts};
}

/*
   This function expands or shrinks **da** into a **n** objects
   big dynarray

   If reallocation fails then 1 is returned and
   the dynarray is left untouched
 */
uint8_t darealloc(dynarray_t *da, size_t n) {
    assert(da != NULL && n != 0);

    size_t new_size = n * da->ts;

    void *tmp = realloc(da->ptr, new_size);
    if (tmp == NULL) return 1; 

    da->ptr = tmp;
    da->size = new_size;
    return 0;
}
/*
   This function frees the data stored by **da** and 
   sets da->ptr to NULL
 */
void dafree(dynarray_t *da) {
    assert(da != NULL);
    free(da->ptr);
    da->ptr = NULL;
}

#endif

/*
   This function returns ! if **da** is safe
   to access, and 0 otherwise
 */
static inline int8_t davalid(dynarray_t *da) {
    assert(da != NULL);
    return (da->ptr != NULL);
}

/*
   This function access the **index** element from  **da**

   A pointer to the indexed element is returned, if
   the element cannot be accessed then NULL is returned.

   - This function performs bounds checking for safety reasons
 */
static inline void *daget(dynarray_t *da, size_t index) {
    assert(da != NULL);

    if (index >= (da->size / da->ts)) return NULL;
    return (((char *) da->ptr) + index *da->ts);
}

/*
   This function access the **index** element from **da**

   A pointer to the indexed element is returned

   - This function does NOT perform bounds checking for performance reasons 
 */
static inline void *dauget(dynarray_t *da, size_t index) {
    assert(da != NULL);
    return (((char *) da->ptr) + index * da->ts);
}


/*
   This function sets the **index** element from **da** with **value**. 
   - This function performs bounds checking for safety reasons.

   This function returns 0 on success.

   If the bounds of the dynarray are violated then 1 is returned
 */
static inline uint8_t daset(dynarray_t *da, size_t index, const void *value) {
    assert(da != NULL && da->ptr != NULL);
    assert(value != NULL);

    if (index >= (da->size / da->ts)) return 1;

    char *ptr = da->ptr;
    memcpy(ptr + index * da->ts, value, da->ts);
    return 0;
}

/*
   This function sets the **index** element from **da** with **value**. 
   - This function does NOT perform bounds checking for performance reasons 
 */
static inline void dauset(dynarray_t *da, size_t index, const void *value) {
    assert(da != NULL && da->ptr != NULL);
    assert(value != NULL);

    char *ptr = da->ptr;
    memcpy(ptr + index * da->ts, value, da->ts);
}


/*
   This function returns the size in bytes of **da** 
 */
static inline size_t dasize(const dynarray_t *da) {
    assert(da != NULL);
    return da->size;
}

/*
   This function returns the total number of objects
   that can be stored in **da**
 */
static inline size_t daobjects(const dynarray_t *da) {
    assert(da != NULL);
    return da->size / da->ts;
}
