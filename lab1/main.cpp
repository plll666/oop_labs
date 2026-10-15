#include "array_ops.h"
#include <iostream>

namespace {

bool read_size(std::size_t& out) {
    if (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Error: expected a non-negative integer.\n";
        return false;
    }
    return true;
}

bool read_int(int& out) {
    if (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Error: expected an integer.\n";
        return false;
    }
    return true;
}

}

int main() {
    int* arr = nullptr;
    std::size_t size = 0;
    int choice = -1;

    do {
        std::cout << "\nMenu\n"
                  << "1. Create array\n"
                  << "2. Print\n"
                  << "3. Insert element\n"
                  << "4. Remove element\n"
                  << "5. Resize\n"
                  << "6. Print\n"
                  << "7. Variant algorithm (insertion sort + rotate left by k)\n"
                  << "8. Binary search\n"
                  << "0. Exit\n"
                  << "Choice: ";

        int input = 0;
        if (!(std::cin >> input)) {
            if (std::cin.eof()) {
                std::cout << "\nInput ended. Exiting.\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Try again.\n";
            continue;
        }
        choice = input;

        switch (choice) {
        case 1: {
            std::size_t n;
            std::cout << "Array size: ";
            if (!read_size(n)) break;

            array_delete(arr);
            arr = array_create(n);
            size = n;

            std::cout << "Enter " << n << " elements:\n";
            for (std::size_t i = 0; i < n; ++i) {
                int v = 0;
                read_int(v);
                arr[i] = v;
            }
            std::cout << "Array created.\n";
            break;
        }
        case 2:
        case 6: {
            if (arr == nullptr) {
                std::cout << "Array is not created.\n";
            } else {
                std::cout << "Array (size=" << size << "): ";
                array_print(arr, size);
            }
            break;
        }
        case 3: {
            if (arr == nullptr) { std::cout << "Create the array first.\n"; break; }
            std::size_t pos;
            int value;
            std::cout << "Position (0.." << size << "): ";
            if (!read_size(pos)) break;
            if (pos > size) { std::cout << "Position is out of range.\n"; break; }
            std::cout << "Value: ";
            if (!read_int(value)) break;
            arr = array_insert(arr, size, pos, value);
            std::cout << "Inserted. New size: " << size << "\n";
            break;
        }
        case 4: {
            if (arr == nullptr || size == 0) { std::cout << "Array is empty.\n"; break; }
            std::size_t pos;
            std::cout << "Position (0.." << size - 1 << "): ";
            if (!read_size(pos)) break;
            if (pos >= size) { std::cout << "Position is out of range.\n"; break; }
            arr = array_remove(arr, size, pos);
            std::cout << "Removed. New size: " << size << "\n";
            break;
        }
        case 5: {
            if (arr == nullptr) { std::cout << "Create the array first.\n"; break; }
            std::size_t new_size;
            std::cout << "New size: ";
            if (!read_size(new_size)) break;
            arr = array_resize(arr, size, new_size);
            size = new_size;
            std::cout << "Size changed to " << size << "\n";
            break;
        }
        case 7: {
            if (arr == nullptr || size == 0) { std::cout << "Array is empty.\n"; break; }
            std::size_t k;
            std::cout << "Rotate left by k: ";
            if (!read_size(k)) break;
            std::cout << "Before sort: "; array_print(arr, size);
            array_insertion_sort(arr, size);
            std::cout << "After sort: "; array_print(arr, size);
            array_rotate_left(arr, size, k);
            std::cout << "After rotate: "; array_print(arr, size);
            break;
        }
        case 8: {
            if (arr == nullptr || size == 0) { std::cout << "Array is empty.\n"; break; }
            std::cout << "(hint: the array must be sorted - use item 7)\n";
            int target;
            std::cout << "Value to search: ";
            if (!read_int(target)) break;
            std::size_t index = 0;
            if (array_binary_search(arr, size, target, index)) {
                std::cout << "Found at index " << index << "\n";
            } else {
                std::cout << "Not found.\n";
            }
            break;
        }
        case 0:
            std::cout << "Exit.\n";
            break;
        default:
            std::cout << "No such menu item.\n";
            break;
        }
    } while (choice != 0);

    array_delete(arr);
    return 0;
}