#include <stdlib.h>
#include "vec.h"


/* Create vector of specified length */
vec *new_vec(size_t len)
{
    /* Allocate header structure */
    // Allocating Memory for the Structure:
    // The malloc function is used to allocate memory dynamically for a vec structure.
    vec *result = (vec *) malloc(sizeof(vec));        
    if (!result)
        return NULL;  /* Couldn't allocate storage */
    
    //Initializing the Structure:
    result->len = len;
    data_t *data = NULL;
    
    // Allocating Memory for the Data Array:
    // calloc(len, sizeof(data_t)) allocates memory for len elements, each of size sizeof(data_t), and initializes the allocated memory to zero.
    // If the allocation fails (i.e., calloc returns NULL), the function frees the previously allocated memory for the vec structure (result) to avoid a memory leak, and then returns NULL.
    if (len > 0) {
        data = (data_t *) calloc(len, sizeof(data_t));
        if (!data) {
            free((void *) result);
            return NULL; /* Couldn't allocate storage */
        }
    }
    /* data will either be NULL or allocated array */
    result->data = data;
    return result;
}

/* Free storage used by vector */
void free_vec(vec *v) {
    if (v->data)
	free(v->data);
    free(v);
}

/*
 * Retrieve vector element and store at dest.
 * Return 0 (out of bounds) or 1 (successful)
 */
int get_vec_element(vec *v, size_t index, data_t *dest)
{
    if (index >= v->len)
	return 0;
    *dest = v->data[index];
    return 1;
}

/* Return length of vector */
size_t vec_length(vec *v)
{
    return v->len;
}


data_t *get_vec_start(vec *v)
{
    return v->data;
}

/*
 * Set vector element.
 * Return 0 (out of bounds) or 1 (successful)
 */
int set_vec_element(vec *v, size_t index, data_t val)
{
    if (index >= v->len)
	return 0;
    v->data[index] = val;
    return 1;
}

