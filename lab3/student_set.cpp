#include "student_set.h"

#include <new>
#include <utility>

StudentSet::StudentSet() : data_(nullptr), size_(0), capacity_(0) {}

StudentSet::StudentSet(const StudentSet& other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ == 0) {
        return;
    }
    data_ = new Student[capacity_];
    try {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    } catch (...) {
        delete[] data_;
        throw;
    }
}

StudentSet::StudentSet(StudentSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

StudentSet& StudentSet::operator=(const StudentSet& other) {
    if (this != &other) {
        StudentSet copy(other);
        swap(copy);
    }
    return *this;
}

StudentSet& StudentSet::operator=(StudentSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

StudentSet::~StudentSet() {
    delete[] data_;
}

void StudentSet::grow() {
    const std::size_t max_capacity = static_cast<std::size_t>(-1) / sizeof(Student);
    if (capacity_ == max_capacity) {
        throw std::bad_array_new_length();
    }
    const std::size_t new_capacity = capacity_ == 0 ? 1
        : capacity_ > max_capacity / 2 ? max_capacity : capacity_ * 2;
    Student* new_data = new Student[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

bool StudentSet::add(const Student& student) {
    if (contains(student)) {
        return false;
    }
    Student copy(student);
    if (size_ == capacity_) {
        grow();
    }
    data_[size_] = std::move(copy);
    ++size_;
    return true;
}

bool StudentSet::remove(const Student& student) {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == student) {
            for (std::size_t j = i + 1; j < size_; ++j) {
                data_[j - 1] = std::move(data_[j]);
            }
            --size_;
            data_[size_] = Student();
            return true;
        }
    }
    return false;
}

bool StudentSet::contains(const Student& student) const {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == student) {
            return true;
        }
    }
    return false;
}

bool StudentSet::operator()(const Student& student) const {
    return contains(student);
}

std::size_t StudentSet::size() const {
    return size_;
}

bool StudentSet::empty() const {
    return size_ == 0;
}

void StudentSet::swap(StudentSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

StudentSet& StudentSet::operator+=(const Student& student) {
    add(student);
    return *this;
}

StudentSet& StudentSet::operator-=(const Student& student) {
    remove(student);
    return *this;
}

StudentSet StudentSet::operator|(const StudentSet& other) const {
    StudentSet result(*this);
    for (std::size_t i = 0; i < other.size_; ++i) {
        result.add(other.data_[i]);
    }
    return result;
}

StudentSet StudentSet::operator&(const StudentSet& other) const {
    StudentSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

StudentSet StudentSet::operator-(const StudentSet& other) const {
    StudentSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

bool StudentSet::operator==(const StudentSet& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            return false;
        }
    }
    return true;
}

bool StudentSet::operator!=(const StudentSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const StudentSet& set) {
    os << '{';
    if (!set.empty()) {
        os << ' ';
        for (std::size_t i = 0; i < set.size_; ++i) {
            if (i != 0) {
                os << ", ";
            }
            os << set.data_[i];
        }
        os << ' ';
    }
    return os << '}';
}
