#include "array_ops.h"

#include <cstddef>
#include <gtest/gtest.h>

namespace {

int* make_array(const int* data, std::size_t n) {
    int* arr = array_create(n);
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = data[i];
    }
    return arr;
}

}

TEST(ArrayCreateDelete, CreateZeroesElements) {
    int* arr = array_create(5);
    ASSERT_NE(arr, nullptr);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], 0) << "element " << i << " is not zeroed";
    }
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

TEST(ArrayCreateDelete, DeleteNullsPointer) {
    const int data[] = {1, 2, 3};
    int* arr = make_array(data, 3);
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

TEST(ArrayCreateDelete, EmptyArrayCreateDeleteSafe) {
    int* arr = array_create(0);
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

TEST(ArrayResize, GrowKeepsValuesAndZeroesTail) {
    const int data[] = {10, 20, 30};
    int* arr = make_array(data, 3);
    std::size_t size = 3;

    arr = array_resize(arr, size, 6);
    size = 6;

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
    EXPECT_EQ(arr[3], 0) << "new cells must be zeroed";
    EXPECT_EQ(arr[4], 0);
    EXPECT_EQ(arr[5], 0);

    array_delete(arr);
}

TEST(ArrayResize, ShrinkKeepsPrefix) {
    const int data[] = {1, 2, 3, 4, 5};
    int* arr = make_array(data, 5);
    std::size_t size = 5;

    arr = array_resize(arr, size, 2);
    size = 2;

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);

    array_delete(arr);
}

TEST(ArrayResize, ShrinkToZero) {
    const int data[] = {7, 8};
    int* arr = make_array(data, 2);
    std::size_t size = 2;

    arr = array_resize(arr, size, 0);
    size = 0;

    EXPECT_EQ(size, 0u);
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

TEST(ArrayInsert, IntoMiddle) {
    const int data[] = {10, 20, 30, 40};
    int* arr = make_array(data, 4);
    std::size_t size = 4;

    arr = array_insert(arr, size, 2, 99);

    ASSERT_EQ(size, 5u);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 99);
    EXPECT_EQ(arr[3], 30);
    EXPECT_EQ(arr[4], 40);

    array_delete(arr);
}

TEST(ArrayInsert, AtFront) {
    const int data[] = {1, 2, 3};
    int* arr = make_array(data, 3);
    std::size_t size = 3;

    arr = array_insert(arr, size, 0, 42);

    ASSERT_EQ(size, 4u);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], 1);
    EXPECT_EQ(arr[3], 3);

    array_delete(arr);
}

TEST(ArrayInsert, AtEnd) {
    const int data[] = {1, 2, 3};
    int* arr = make_array(data, 3);
    std::size_t size = 3;

    arr = array_insert(arr, size, 3, 77);

    ASSERT_EQ(size, 4u);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 77);

    array_delete(arr);
}

TEST(ArrayInsert, InvalidPositionKeepsArray) {
    const int data[] = {5, 6};
    int* arr = make_array(data, 2);
    std::size_t size = 2;

    arr = array_insert(arr, size, 3, 100);

    EXPECT_EQ(size, 2u);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 6);

    array_delete(arr);
}

TEST(ArrayRemove, FromMiddle) {
    const int data[] = {1, 2, 3, 4, 5};
    int* arr = make_array(data, 5);
    std::size_t size = 5;

    arr = array_remove(arr, size, 1);

    ASSERT_EQ(size, 4u);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);
    EXPECT_EQ(arr[2], 4);
    EXPECT_EQ(arr[3], 5);

    array_delete(arr);
}

TEST(ArrayRemove, RemoveLastLeavesEmpty) {
    const int data[] = {42};
    int* arr = make_array(data, 1);
    std::size_t size = 1;

    arr = array_remove(arr, size, 0);

    EXPECT_EQ(size, 0u);
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

TEST(ArrayRemove, InvalidPositionKeepsArray) {
    const int data[] = {1, 2};
    int* arr = make_array(data, 2);
    std::size_t size = 2;

    arr = array_remove(arr, size, 2);

    EXPECT_EQ(size, 2u);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);

    array_delete(arr);
}

TEST(BinarySearch, FoundIndex) {
    const int data[] = {1, 3, 5, 7, 9};
    int* arr = make_array(data, 5);

    std::size_t index = 999;
    ASSERT_TRUE(array_binary_search(arr, 5, 7, index));
    EXPECT_EQ(index, 3u);

    index = 999;
    ASSERT_TRUE(array_binary_search(arr, 5, 1, index));
    EXPECT_EQ(index, 0u);

    array_delete(arr);
}

TEST(BinarySearch, NotFound) {
    const int data[] = {1, 3, 5, 7, 9};
    int* arr = make_array(data, 5);

    std::size_t index = 999;
    EXPECT_FALSE(array_binary_search(arr, 5, 4, index));

    array_delete(arr);
}

TEST(BinarySearch, EmptyArrayReturnsFalse) {
    int* arr = array_create(0);

    std::size_t index = 999;
    EXPECT_FALSE(array_binary_search(arr, 0, 10, index));

    array_delete(arr);
}

TEST(InsertionSort, ControlExample) {
    const int data[] = {4, 2, 5, 1, 3};
    const int expected[] = {1, 2, 3, 4, 5};
    int* arr = make_array(data, 5);

    array_insertion_sort(arr, 5);

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "index " << i;
    }

    array_delete(arr);
}

TEST(InsertionSort, SortedArrayStaysSortedAndEmptySafe) {
    const int data[] = {1, 2, 3};
    int* arr = make_array(data, 3);
    array_insertion_sort(arr, 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    array_delete(arr);

    int* empty = array_create(0);
    array_insertion_sort(empty, 0);
    array_delete(empty);
}

TEST(RotateLeft, VariantControlExample) {
    const int data[] = {1, 2, 3, 4, 5};
    const int expected[] = {3, 4, 5, 1, 2};
    int* arr = make_array(data, 5);

    array_rotate_left(arr, 5, 2);

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "index " << i;
    }

    array_delete(arr);
}

TEST(RotateLeft, EdgeCases) {
    const int d0[] = {1, 2, 3};
    int* a0 = make_array(d0, 3);
    array_rotate_left(a0, 3, 0);
    EXPECT_EQ(a0[0], 1);
    EXPECT_EQ(a0[1], 2);
    EXPECT_EQ(a0[2], 3);
    array_delete(a0);

    const int d1[] = {1, 2, 3};
    int* a1 = make_array(d1, 3);
    array_rotate_left(a1, 3, 3);
    EXPECT_EQ(a1[0], 1);
    EXPECT_EQ(a1[1], 2);
    EXPECT_EQ(a1[2], 3);
    array_delete(a1);

    const int d2[] = {1, 2, 3, 4};
    const int e2[] = {2, 3, 4, 1};
    int* a2 = make_array(d2, 4);
    array_rotate_left(a2, 4, 5);
    for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(a2[i], e2[i]) << "index " << i;
    }
    array_delete(a2);
}

TEST(RotateLeft, EmptyArraySafe) {
    int* arr = array_create(0);
    array_rotate_left(arr, 0, 3);
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}