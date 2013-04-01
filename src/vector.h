#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @mainpage Yet another vector library for C language
 *
 * @par Introduction
 * This vector library provides functions for C language.
 * This library is characterized as following:
 * @li no source codes, only a header
 * @li correspond to any type
 * @li APIs of library are similar to Vector's one in Java
 *
 * And this library is the following restrictions:
 * @li create only one type vector in your one source code
 * @li all library APIs are implemented on macro
 *
 * These restrictions are due to the need to respond any type at this library.
 *
 * @par License
 * This library is free software distributed under the terms of the MIT license
 * reproduced here.
 *
 * @par Install
 * Copy vector.h file to your source code tree.
 *
 * @par How to use
 * Please see #vector_init and #vector_alloc in this document.
 * The following source code is test case of this library.
 *
 * @includelineno test.c
 *
 * @section history_sec History
 *
 * @par Ver 0.2.0: March 31, 2013
 * @li Added #to_array function
 * @li Changed specification of vector capacity
 * @li Changed #vector_remove_internal to static (bug fix)
 *
 * @par Ver 0.1.0: March 29, 2013
 * @li First release
 */

/**
 * @file
 * The vector functions implements a growable array of any data types.
 * Like an array, it contains components that can be accessed using an integer
 * index. However, the size of a vector can grow or shrink as needed to
 * accommodate adding and removing items after the vector has been created.
 *
 * Each vector tries to optimize storage management by maintaining a capacity
 * and a capacity_increment. The capacity is always at least as large as the
 * vector size; it is usually larger because as components are added to the
 * vector, the vector's storage increases in chunks the size of
 * capacity_increment. An application can increase the capacity of a vector
 * before inserting a large number of components; this reduces the amount of
 * incremental reallocation.
 */

/**
 * Memory allocator of this vector functions.
 * You have to implement this function in your source code like the following:
 *
 * @code
 * static void *vector_alloc(void *ptr, size_t size) {
 *     if (size == UINT_MAX) {
 *         free(ptr);
 *         return NULL;
 *     }
 *     return realloc(ptr, size);
 * }
 * @endcode
 *
 * This function affects to release memory block if size specified UINT_MAX.
 *
 * @param [in] ptr pointer to a memory block previously allocated with this
 * function. Alternatively, this can be a null pointer, in which case a new
 * block is allocated
 * @param [in] size new size for the memory block, in bytes.
 * Alternatively, this function releases memory block if size is UINT_MAX
 * @return a pointer to the reallocated memory block, which may be either the
 * same as ptr or a new location. Or a null pointer if systems does not enough
 * memory or size specified UINT_MAX
 * @see #vector_init
 */
static void *vector_alloc(void *ptr, size_t size);

/**
 * Create a new vector.
 *
 * @param [in] capacity capacity of a new vector.
 * vector capacity is one if specified capacity is zero
 * @param [in] capacity_increment the amount by which the capacity of the vector
 * is automatically incremented when its size becomes greater than its capacity.
 * If the capacity_increment is equal to zero, the capacity of the vector is
 * doubled each time it needs to grow
 * @return a pointer of new vector if the new vector was created, otherwise NULL
 */
#define vector_create(capacity, capacity_increment) \
	vector_alloc_internal(NULL, capacity, capacity_increment)

/**
 * Destroy specified vector.
 *
 * @param [in,out] v specified vector
 */
#define vector_destroy(v) \
	if (v) vector_alloc(((vector *) v)->elements, UINT_MAX), vector_alloc(v, UINT_MAX)

/**
 * Returns the number of components in specified vector.
 *
 * @param [in] v specified vector
 * @return the number of components in specified vector
 */
#define vector_size(v) (v)->size

/**
 * Returns the current capacity of specified vector.
 *
 * @param [in] v specified vector
 * @return the current capacity (the length of its internal data array,
 * kept in the member elements of specified vector)
 */
#define vector_capacity(v) (v)->capacity

/**
 * Appends the specified element to the end of specified vector.
 *
 * This function fails in the following cases:
 * @li the specified element is invalid value
 * @li does not enough memory
 *
 * @param [in,out] v specified vector
 * @param [in] element element to be appended to specified vector
 * @return true if the specified element was appended, otherwise false
 */
#define vector_add(v, element) vector_insert(v, (v)->size, element)

/**
 * Inserts the specified element at the specified position in specified vector.
 * Shifts the element currently at that position (if any)
 * and any subsequent elements to the right (adds one to their indices).
 *
 * This function fails in the following cases:
 * @li the specified element is invalid value
 * @li the specified index is out of range (index < 0 || index > vector_size(v))
 * @li does not enough memory
 *
 * @param [in,out] v specified vector
 * @param [in] index index at which the specified element is to be inserted
 * @param [in] element element to be inserted
 * @return true if the specified element was appended, otherwise false
 */
#define vector_insert(v, index, element) vector_insert_internal(v, index, element)

/**
 * Replaces the element at the specified position in specified vector with the
 * specified element.
 *
 * @param [in,out] v specified vector
 * @param [in] index index of the element to replace
 * @param [in] element element to be stored at the specified position
 * @return the element previously at the specified position
 */
#define vector_set(v, index, element) (v)->elements[index] = element

/**
 * Removes the element at the specified position in specified vector.
 * Shifts any subsequent elements to the left (subtracts one from their
 * indices). Returns the element that was removed from the vector.
 *
 * @param [in,out] v specified vector
 * @param [in] index the index of the element to be removed
 * @return element that was removed
 */
#define vector_remove(v, index) vector_remove_internal(v, index)

/**
 * Returns the element at the specified position in specified vector.
 *
 * @param [in] v specified vector
 * @param [in] index index of the element to return
 * @return element at the specified index
 */
#define vector_get(v, index) (v)->elements[index]

/**
 * Tests if specified vector has no components.
 *
 * @param [in] v specified vector
 * @return true if and only if specified vector has no components,
 * that is, its size is zero; false otherwise
 */
#define vector_is_empty(v) vector_size(v) == 0

/**
 * Returns the index of the first occurrence of the specified element in
 * specified vector, or -1 if specified vector does not contain the element.
 *
 * @param [in] v specified vector
 * @param [in] element element to search for
 * @return the index of the first occurrence of the specified element in
 * specified vector, or -1 if specified vector does not contain the element or
 * specified vector size more than INT_MAX
 */
#define vector_index_of(v, element) vector_index_of_internal(v, element)

/**
 * Returns true if specified vector contains the specified element.
 *
 * @param [in] v specified vector
 * @param [in] element element whose presence in specified vector is to be
 * tested
 * @return true if specified vector contains the specified element
 */
#define vector_contains(v, element) vector_index_of(v, element) != -1

/**
 * Removes all of the elements from specified vector.
 * The vector will be empty after this call returns (unless it throws an
 * exception).
 *
 * @param [in,out] v specified vector
 */
#define vector_clear(v) vector_clear_internal(v)

/**
 * Returns an array containing all of the elements in specified vector in the
 * correct order.
 *
 * Element that position is vector size is always invalid value. For example,
 * the following sample code is valid.
 *
 * @code
 * vector_init(unsigned char, '\0');
 * vector *v = vector_create(3, 1);
 * vector_add(v, 'a');
 * vector_add(v, 'b');
 * vector_add(v, 'c');
 * printf("%s\n", vector_to_array(v));
 * @endcode
 *
 * @param [in] v specified vector
 * @return an array containing all of the elements in this collection, or NULL
 * if systems does not enough memory
 */
#define vector_to_array(v) vector_to_array_internal(v)

/**
 * Initialize functions of specified type vector.
 *
 * You have to write in your source code like the following:
 *
 * @code
 * #include "vector.h"
 *
 * vector_init(int, UINT_MAX);
 * @endcode
 *
 * Or:
 *
 * @code
 * #include "vector.h"
 *
 * vector_init(char *, NULL);
 * @endcode
 *
 * @param [in] type type of variable for vector elements
 * @param [in] invalid invalid value for vector. All vector objects never have
 * this element
 * @see #vector_alloc
 */
#define vector_init(type, invalid) \
	typedef struct { \
		size_t size; \
		size_t capacity; \
		size_t capacity_increment; \
		type *elements; \
	} vector; \
	\
	static vector *vector_alloc_internal(vector *v, size_t capacity, size_t capacity_increment) { \
		if (v == NULL) { \
			v = (vector *) vector_alloc(NULL, sizeof(vector)); \
			if (v != NULL) { \
				v->size = 0; \
				v->capacity = 1; \
				v->capacity_increment = capacity_increment; \
				v->elements = NULL; \
				vector_alloc_internal(v, capacity, capacity_increment); \
			} \
		} else { \
			v->elements = (type *) vector_alloc(v->elements, capacity * sizeof(type)); \
			if (v->elements) { \
				size_t i; \
				for (i = v->capacity; i < capacity; i++) { \
					v->elements[i] = invalid; \
				} \
				if (capacity != 0) { \
					v->capacity = capacity; \
				} \
			} else { \
				v = NULL; \
			} \
		} \
		return v; \
	} \
	\
	static int vector_insert_internal(vector *v, size_t index, type element) { \
		int result = 0; \
		if (v->size < v->capacity - 1) { \
			if (element != invalid) { \
				size_t i; \
				for (i = v->size; i > index; i--) { \
					v->elements[i] = v->elements[i - 1]; \
				} \
				v->elements[index] = element; \
				v->size++; \
				result = 1; \
			} \
		} else { \
			size_t new_capacity; \
			if (v->capacity_increment) { \
				new_capacity = v->capacity + v->capacity_increment; \
			} else { \
				new_capacity = v->capacity * 2; \
			} \
			if (vector_alloc_internal(v, new_capacity, v->capacity_increment)) { \
				result = vector_insert_internal(v, index, element); \
			} \
		} \
		return result; \
	} \
	\
	static type vector_remove_internal(vector *v, size_t index) { \
		type element = v->elements[index]; \
		size_t i; \
		for (i = index + 1; i < v->size; i++) { \
			v->elements[i - 1] = v->elements[i]; \
		} \
		v->size--; \
		v->elements[v->size] = invalid; \
		return element; \
	} \
	\
	static int vector_index_of_internal(vector *v, type element) { \
		size_t i, n = v->size < INT_MAX ? v->size : INT_MAX; \
		for (i = 0; i < n; i++) { \
			if (v->elements[i] == element) { \
				return i; \
			} \
		} \
		return -1; \
	} \
	\
	static void vector_clear(vector *v) { \
		while (v->size) { \
			v->elements[--(v->size)] = invalid; \
		} \
	} \
	\
	static const type *vector_to_array_internal(vector *v) { \
		return (const type *) v->elements; \
	}

#endif
