#include <iostream>
#include <utility>

#include "lab_string.h"

int main() {
    String s1;
    String s2("Hello");
    String s3(s2);
    String s4(std::move(s3));

    s1 = s2;
    s1.append(", World!");
    s1.print(std::cout);
    std::cout << '\n';

    const std::size_t pos = s1.find("World");
    std::cout << "find(\"World\") = " << pos << '\n';

    std::cout << "count_char('l') = " << s1.count_char('l') << '\n';
    std::cout << "s4 = ";
    s4.print(std::cout);
    std::cout << '\n';

    return 0;
}
