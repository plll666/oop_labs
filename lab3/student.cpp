#include "student.h"

#include <cstring>
#include <utility>

Student::Student() : name_(nullptr), id_(0) {}

Student::Student(const char* name, int id) : name_(nullptr), id_(id) {
    const char* text = name ? name : "";
    name_ = new char[std::strlen(text) + 1];
    std::strcpy(name_, text);
}

Student::Student(const Student& other) : Student(other.name(), other.id_) {}

Student::Student(Student&& other) noexcept : name_(other.name_), id_(other.id_) {
    other.name_ = nullptr;
    other.id_ = 0;
}

Student& Student::operator=(const Student& other) {
    if (this != &other) {
        Student copy(other);
        swap(copy);
    }
    return *this;
}

Student& Student::operator=(Student&& other) noexcept {
    if (this != &other) {
        delete[] name_;
        name_ = other.name_;
        id_ = other.id_;
        other.name_ = nullptr;
        other.id_ = 0;
    }
    return *this;
}

Student::~Student() {
    delete[] name_;
}

const char* Student::name() const {
    return name_ ? name_ : "";
}

int Student::id() const {
    return id_;
}

void Student::swap(Student& other) noexcept {
    std::swap(name_, other.name_);
    std::swap(id_, other.id_);
}

bool Student::operator==(const Student& other) const {
    return id_ == other.id_;
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    return os << '(' << student.id_ << ", " << student.name() << ')';
}
