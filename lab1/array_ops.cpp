#include "array_ops.h"
#include <iostream> 


namespace {

void reverse_range(int* arr, std::size_t lo, std::size_t hi) {
    while (lo < hi && lo < --hi) {
        int tmp = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = tmp;
        ++lo;
    }
}

}

int* array_create(std::size_t size) {
    int* arr = new int[size];
    for (std::size_t i = 0; i < size; ++i) {
        arr[i] = 0;
    }

    return arr;
}

void array_delete(int*& arr) {
    delete[] arr;
    arr = nullptr;
}

int* array_resize(int* arr, std::size_t size, std::size_t new_size){
    int* new_arr = new int[new_size];
    std::size_t min_size = (size < new_size) ? size : new_size;

    for (std::size_t i = 0; i < min_size; ++i) {
        new_arr[i] = arr[i];
    }

    for (std::size_t i = min_size; i < new_size; ++i) {
        new_arr[i] = 0;
    }

    delete[] arr;
    return new_arr;
}

int* array_insert(int* arr, std::size_t& size, std::size_t pos, int value) {
    if (pos > size) {
        return arr;
    }

    int* new_arr = new int[size + 1];

    for (std::size_t i = 0; i < pos; ++i) {
        new_arr[i] = arr[i];
    }

    new_arr[pos] = value;

    for (std::size_t i = pos; i < size; ++i) {
        new_arr[i + 1] = arr[i];
    }

    delete[] arr;
    ++size;
    return new_arr;
}

int* array_remove(int* arr, std::size_t& size, std::size_t pos) {
    if (pos >= size) {
        return arr;
    }

    int* new_arr = new int[size - 1];

    for (std::size_t i = 0; i < pos; ++i) {
        new_arr[i] = arr[i];
    }

    for (std::size_t i = pos + 1; i < size; ++i) {
        new_arr[i - 1] = arr[i];
    }

    delete[] arr;
    --size;
    return new_arr;
}

void array_print(const int* arr, std::size_t size) {
    if (size == 0) { std::cout << "empty\n"; return; }
    std::cout << "[";
    for (std::size_t i = 0; i < size; ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << arr[i];
    }
std::cout << "]\n";
}

bool array_binary_search(const int* arr, std::size_t size, int target, std::size_t& out_index) {
    std::size_t left = 0;
    std::size_t right = size;

    while (left < right) {
        std::size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            out_index = mid;
            return true;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return false;
}

void array_insertion_sort(int* arr, std::size_t size) {
    if (size < 2) {
        return;
    }

    for (std::size_t i = 1; i < size; ++i) {
        int key = arr[i];
        std::size_t j = i;

        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }

        arr[j] = key;
    }
}

void array_rotate_left(int* arr, std::size_t size, std::size_t k) {
    if (size == 0) {
        return;
    }

    k %= size;
    if (k == 0) {
        return;
    }

    reverse_range(arr, 0, size);
    reverse_range(arr, 0, size - k);
    reverse_range(arr, size - k, size);
}