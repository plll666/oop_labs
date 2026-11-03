#include "student_set.h"

#include <iostream>
#include <utility>

int main() {
    StudentSet a;
    a += Student("Ivan", 1);
    a += Student("Anna", 2);
    a += Student("Petr", 1);

    StudentSet b;
    b += Student("Anna", 2);
    b += Student("Oleg", 3);

    std::cout << "A = " << a << '\n';
    std::cout << "B = " << b << '\n';
    std::cout << "A | B = " << (a | b) << '\n';
    std::cout << "A & B = " << (a & b) << '\n';
    std::cout << "A - B = " << (a - b) << '\n';

    StudentSet copy(a);
    copy -= Student("", 1);
    std::cout << "Copy after remove = " << copy << '\n';
    std::cout << "A after copying = " << a << '\n';

    StudentSet moved(std::move(copy));
    std::cout << "Moved = " << moved << '\n';
    std::cout << "Source after move = " << copy << '\n';
}
