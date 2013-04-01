#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <CUnit.h>
#include <Console.h>

#include "vector.h"

vector_init(int, INT_MAX);

static void *vector_alloc(void *ptr, size_t size) {
	if (size == UINT_MAX) {
		free(ptr);
		return NULL;
	}
	return realloc(ptr, size);
}

static void test_create_01() {
	vector *v = vector_create(0, 1);
	CU_ASSERT_PTR_NOT_NULL_FATAL(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_NOT_EQUAL(0, vector_capacity(v));
	vector_destroy(v);
}

static void test_create_02() {
	vector *v = vector_create(1, 1);
	CU_ASSERT_PTR_NOT_NULL_FATAL(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_EQUAL(1, vector_capacity(v));
	vector_destroy(v);
}

static void test_create_03() {
	vector *v = vector_create(0, 1);
	vector *v2 = vector_create(0, 1);
	CU_ASSERT_PTR_NOT_NULL_FATAL(v);
	CU_ASSERT_PTR_NOT_NULL_FATAL(v2);
	vector_destroy(v);
	vector_destroy(v2);
}

static void test_create_04() {
	int i;
	for (i = 0; i < 100; i++) {
		vector *v = vector_create(0, 1);
		CU_ASSERT_PTR_NOT_NULL_FATAL(v);
		vector_destroy(v);
	}
}

static void test_create_05() {
	int i;
	vector *vs[100];
	for (i = 0; i < 100; i++) {
		vs[i] = vector_create(0, 1);
		CU_ASSERT_PTR_NOT_NULL_FATAL(vs[i]);
	}
	for (i = 0; i < 100; i++) {
		vector_destroy(vs[i]);
	}
}

static void test_create_06() {
	vector *v = vector_create(0, 3);
	CU_ASSERT_PTR_NOT_NULL_FATAL(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_NOT_EQUAL(0, vector_capacity(v));
	vector_add(v, 123);
	CU_ASSERT_NOT_EQUAL(0, vector_capacity(v));
	vector_add(v, 456);
	CU_ASSERT_NOT_EQUAL(1, vector_capacity(v));
	vector_add(v, 789);
	CU_ASSERT_NOT_EQUAL(2, vector_capacity(v));
	vector_add(v, 234);
	CU_ASSERT_NOT_EQUAL(3, vector_capacity(v));
	vector_destroy(v);
}

static void test_create_07() {
	vector *v = vector_create(1, 0);
	CU_ASSERT_PTR_NOT_NULL_FATAL(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_EQUAL(1, vector_capacity(v));
	vector_add(v, 123);
	CU_ASSERT_EQUAL(2, vector_capacity(v));
	vector_add(v, 456);
	CU_ASSERT_EQUAL(4, vector_capacity(v));
	vector_add(v, 789);
	CU_ASSERT_EQUAL(4, vector_capacity(v));
	vector_destroy(v);
}

static void test_create_08() {
	vector *v = vector_create(1, 1024 * 1024);
	size_t capacity;
	CU_ASSERT_PTR_NOT_NULL_FATAL(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_EQUAL(1, vector_capacity(v));
	vector_add(v, 123);
	capacity = vector_capacity(v);
	CU_ASSERT_TRUE(capacity > 1024 * 1024);
	vector_add(v, 456);
	CU_ASSERT_EQUAL(capacity, vector_capacity(v));
	vector_add(v, 789);
	CU_ASSERT_EQUAL(capacity, vector_capacity(v));
	vector_destroy(v);
}

static void test_destroy_01() {
	vector *v = vector_create(0, 1);
	vector_destroy(v);
}

static void test_destroy_02() {
	vector_destroy(NULL);
}

static void test_size_01() {
	vector *v = vector_create(0, 1);
	int i;
	CU_ASSERT_EQUAL(0, vector_size(v));
	for (i = 0; i < 100; i++) {
		vector_add(v, i);
		CU_ASSERT_EQUAL(i + 1, vector_size(v));
	}
	vector_destroy(v);
}

static void test_capacity_01() {
	int capacity = 1;
	vector *v = vector_create(capacity, 1);
	int i;
	CU_ASSERT_EQUAL(capacity, vector_capacity(v));
	for (i = 0; i < 100; i++) {
		vector_add(v, i);
		CU_ASSERT_TRUE(vector_capacity(v) >= capacity);
		if (vector_capacity(v) > capacity) {
			capacity = vector_capacity(v);
		}
	}
	vector_destroy(v);
}

static void test_add_01() {
	vector *v = vector_create(0, 1);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_NOT_EQUAL(0, vector_capacity(v));
	vector_add(v, 123);
	CU_ASSERT_EQUAL(1, vector_size(v));
	CU_ASSERT_NOT_EQUAL(1, vector_capacity(v));
	CU_ASSERT_EQUAL(123, vector_get(v, 0));
	vector_add(v, 456);
	CU_ASSERT_EQUAL(2, vector_size(v));
	CU_ASSERT_NOT_EQUAL(2, vector_capacity(v));
	CU_ASSERT_EQUAL(123, vector_get(v, 0));
	CU_ASSERT_EQUAL(456, vector_get(v, 1));
	vector_destroy(v);
}

static void test_add_02() {
	vector *v = vector_create(10, 1);
	int i;
	for (i = 0; i < 10; i++) {
		vector_add(v, i);
	}
	CU_ASSERT_EQUAL(10, vector_size(v));
	CU_ASSERT_TRUE(vector_capacity(v) > 10);
	for (i = 0; i < 10; i++) {
		CU_ASSERT_EQUAL(i, vector_get(v, i));
	}
	vector_add(v, 10);
	CU_ASSERT_EQUAL(11, vector_size(v));
	CU_ASSERT_NOT_EQUAL(10, vector_capacity(v));
	CU_ASSERT_EQUAL(10, vector_get(v, 10));
	vector_destroy(v);
}

static void test_add_03() {
	vector *v = vector_create(10, 1);
	int result = vector_add(v, INT_MAX);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_FALSE(result);
	vector_destroy(v);
}

static void test_insert_01() {
	vector *v = vector_create(0, 1);
	vector_insert(v, 0, 123);
	CU_ASSERT_EQUAL(1, vector_size(v));
	CU_ASSERT_NOT_EQUAL(0, vector_capacity(v));
	CU_ASSERT_EQUAL(123, vector_get(v, 0));
	vector_destroy(v);
}

static void test_insert_02() {
	vector *v = vector_create(100, 1);
	int i;
	for (i = 0; i < 100; i++) {
		vector_insert(v, 0, i);
	}
	for (i = 0; i < 100; i++) {
		CU_ASSERT_EQUAL(99 - i, vector_get(v, i));
	}
	vector_destroy(v);
}

static void test_insert_03() {
	vector *v = vector_create(0, 1);
	vector *v2 = vector_create(0, 1);
	int i;
	for (i = 0; i < 100; i++) {
		vector_add(v, i);
		vector_insert(v2, vector_size(v2), i);
	}
	CU_ASSERT_EQUAL(vector_size(v), vector_size(v2));
	CU_ASSERT_EQUAL(vector_capacity(v), vector_capacity(v2));
	for (i = 0; i < 100; i++) {
		CU_ASSERT_EQUAL(vector_get(v, i), vector_get(v2, i));
	}
	vector_destroy(v);
	vector_destroy(v2);
}

static void test_set_01() {
	vector *v = vector_create(100, 1);
	int i;
	for (i = 0; i < vector_capacity(v); i++) {
		vector_set(v, i, i);
		CU_ASSERT_EQUAL(i, vector_get(v, i));
	}
	for (i = 0; i < vector_capacity(v); i++) {
		vector_set(v, i, i * 2);
		CU_ASSERT_EQUAL(i * 2, vector_get(v, i));
	}
	vector_destroy(v);
}

static void test_remove_01() {
	vector *v = vector_create(1, 1);
	int i;
	for (i = 0; i < 100; i++) {
		vector_add(v, i);
		CU_ASSERT_EQUAL(i, vector_get(v, i));
	}
	for (i = 0; i < 100; i++) {
		vector_remove(v, 0);
		if (vector_size(v) != 0) {
			CU_ASSERT_EQUAL(i + 1, vector_get(v, 0));
		}
		CU_ASSERT_EQUAL(99 - i, vector_size(v));
	}
	vector_destroy(v);
}

static void test_remove_02() {
	vector *v = vector_create(1, 1);
	int i;
	for (i = 0; i < 100; i++) {
		vector_add(v, i);
		CU_ASSERT_EQUAL(i, vector_get(v, i));
	}
	while (vector_size(v) != 0) {
		CU_ASSERT_EQUAL(vector_size(v) - 1, vector_get(v, vector_size(v) - 1));
		vector_remove(v, vector_size(v) - 1);
		CU_ASSERT_EQUAL(INT_MAX, vector_get(v, vector_size(v)));
	}
	vector_destroy(v);
}

static void test_get_01() {
	vector *v = vector_create(100, 1);
	int i;
	for (i = 0; i < 50; i++) {
		vector_add(v, i);
		CU_ASSERT_EQUAL(i, vector_get(v, i));
	}
	for (; i < 100; i++) {
		CU_ASSERT_EQUAL(INT_MAX, vector_get(v, i));
	}
	vector_destroy(v);
}

static void test_is_empty_01() {
	vector *v = vector_create(0, 1);
	CU_ASSERT_TRUE(vector_is_empty(v));
	vector_add(v, 123);
	CU_ASSERT_FALSE(vector_is_empty(v));
	vector_remove(v, 0);
	CU_ASSERT_TRUE(vector_is_empty(v));
	vector_destroy(v);
}

static void test_index_of_01() {
	vector *v = vector_create(0, 1);
	CU_ASSERT_EQUAL(-1, vector_index_of(v, 123));
	vector_destroy(v);
}

static void test_index_of_02() {
	vector *v = vector_create(0, 1);
	vector_add(v, 123);
	CU_ASSERT_EQUAL(0, vector_index_of(v, 123));
	vector_add(v, 456);
	CU_ASSERT_EQUAL(0, vector_index_of(v, 123));
	CU_ASSERT_EQUAL(1, vector_index_of(v, 456));
	CU_ASSERT_EQUAL(-1, vector_index_of(v, 789));
	vector_destroy(v);
}

static void test_index_of_03() {
	vector *v = vector_create(0, 1);
	vector_add(v, 123);
	vector_add(v, 123);
	CU_ASSERT_EQUAL(0, vector_index_of(v, 123));
	vector_destroy(v);
}

static void test_index_of_04() {
	vector *v = vector_create(0, 1);
	vector_add(v, 123);
	vector_add(v, 123);
	CU_ASSERT_EQUAL(0, vector_index_of(v, 123));
	vector_destroy(v);
}

static void test_index_of_05() {
	vector *v = vector_create(0, 1);
	vector_add(v, 123);
	vector_remove(v, 0);
	CU_ASSERT_EQUAL(-1, vector_index_of(v, 123));
	vector_destroy(v);
}

static void test_contains_01() {
	vector *v = vector_create(0, 1);
	CU_ASSERT_FALSE(vector_contains(v, 0));
	vector_destroy(v);
}

static void test_contains_02() {
	vector *v = vector_create(0, 1);
	vector_add(v, 123);
	vector_add(v, 456);
	CU_ASSERT_FALSE(vector_contains(v, 0));
	CU_ASSERT_TRUE(vector_contains(v, 123));
	CU_ASSERT_TRUE(vector_contains(v, 456));
	CU_ASSERT_FALSE(vector_contains(v, 789));
	vector_destroy(v);
}

static void test_clear_01() {
	vector *v = vector_create(1, 1);
	vector_clear(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_EQUAL(1, vector_capacity(v));
	vector_destroy(v);
}

static void test_clear_02() {
	vector *v = vector_create(10, 1);
	vector_clear(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_EQUAL(10, vector_capacity(v));
	vector_destroy(v);
}

static void test_clear_03() {
	vector *v = vector_create(10, 1);
	int i;
	for (i = 0; i < 100; i++) {
		vector_add(v, i);
	}
	vector_clear(v);
	CU_ASSERT_EQUAL(0, vector_size(v));
	CU_ASSERT_TRUE(vector_is_empty(v));
	CU_ASSERT_TRUE(vector_capacity(v) >= 100);
	vector_destroy(v);
}

static void test_to_array_01() {
	vector *v = vector_create(1, 1);
	const int *array;
	vector_add(v, 123);
	vector_add(v, 456);
	vector_add(v, 789);
	CU_ASSERT_EQUAL(4, vector_capacity(v));
	array = vector_to_array(v);
	CU_ASSERT_EQUAL(4, vector_capacity(v));
	CU_ASSERT_EQUAL(123, array[0]);
	CU_ASSERT_EQUAL(456, array[1]);
	CU_ASSERT_EQUAL(789, array[2]);
	CU_ASSERT_EQUAL(INT_MAX, array[3]);
	vector_destroy(v);
}

int main(int argc, char *argv[]) {
	CU_pSuite suite;
	CU_initialize_registry();
	suite = CU_add_suite("tests", NULL, NULL);
	CU_add_test(suite, "test_create_01", test_create_01);
	CU_add_test(suite, "test_create_02", test_create_02);
	CU_add_test(suite, "test_create_03", test_create_03);
	CU_add_test(suite, "test_create_04", test_create_04);
	CU_add_test(suite, "test_create_05", test_create_05);
	CU_add_test(suite, "test_create_05", test_create_05);
	CU_add_test(suite, "test_create_06", test_create_06);
	CU_add_test(suite, "test_create_07", test_create_07);
	CU_add_test(suite, "test_create_08", test_create_08);
	CU_add_test(suite, "test_destroy_01", test_destroy_01);
	CU_add_test(suite, "test_destroy_02", test_destroy_02);
	CU_add_test(suite, "test_size_01", test_size_01);
	CU_add_test(suite, "test_capacity_01", test_capacity_01);
	CU_add_test(suite, "test_add_01", test_add_01);
	CU_add_test(suite, "test_add_02", test_add_02);
	CU_add_test(suite, "test_add_03", test_add_03);
	CU_add_test(suite, "test_insert_01", test_insert_01);
	CU_add_test(suite, "test_insert_02", test_insert_02);
	CU_add_test(suite, "test_insert_03", test_insert_03);
	CU_add_test(suite, "test_set_01", test_set_01);
	CU_add_test(suite, "test_remove_01", test_remove_01);
	CU_add_test(suite, "test_remove_02", test_remove_02);
	CU_add_test(suite, "test_get_01", test_get_01);
	CU_add_test(suite, "test_is_empty_01", test_is_empty_01);
	CU_add_test(suite, "test_index_of_01", test_index_of_01);
	CU_add_test(suite, "test_index_of_02", test_index_of_02);
	CU_add_test(suite, "test_index_of_03", test_index_of_03);
	CU_add_test(suite, "test_index_of_04", test_index_of_04);
	CU_add_test(suite, "test_index_of_05", test_index_of_05);
	CU_add_test(suite, "test_contains_01", test_contains_01);
	CU_add_test(suite, "test_contains_02", test_contains_02);
	CU_add_test(suite, "test_clear_01", test_clear_01);
	CU_add_test(suite, "test_clear_02", test_clear_02);
	CU_add_test(suite, "test_clear_03", test_clear_03);
	CU_add_test(suite, "test_to_array_01", test_to_array_01);
	CU_console_run_tests();
	CU_cleanup_registry();
	return EXIT_SUCCESS;
}
