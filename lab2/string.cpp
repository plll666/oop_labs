#include "lab_string.h"

#include <cstring>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace {
constexpr std::size_t max_capacity = std::numeric_limits<std::size_t>::max() - 1;

std::size_t grown_capacity(std::size_t current, std::size_t requested) {
    if (requested > max_capacity) {
        throw std::length_error("String capacity is too large");
    }
    std::size_t cap = current ? current : 1;
    while (cap < requested) {
        if (cap > max_capacity / 2) {
            return requested;
        }
        cap *= 2;
    }
    return cap;
}
}

String::String() : data_(new char[2]), size_(0), capacity_(1) {
    data_[0] = '\0';
}

String::String(const char* s) {
    size_ = s ? std::strlen(s) : 0;
    capacity_ = size_ ? size_ : 1;
    data_ = new char[capacity_ + 1];
    if (size_ != 0) {
        std::memcpy(data_, s, size_);
    }
    data_[size_] = '\0';
}

String::String(const String& other) {
    size_ = other.size_;
    capacity_ = other.capacity_ ? other.capacity_ : 1;
    data_ = new char[capacity_ + 1];
    if (size_ != 0) {
        std::memcpy(data_, other.data_, size_);
    }
    data_[size_] = '\0';
}

String::String(String&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

String::~String() {
    delete[] data_;
}

String& String::operator=(String other) {
    swap(other);
    return *this;
}

void String::swap(String& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::size_t String::size() const {
    return size_;
}

std::size_t String::capacity() const {
    return capacity_;
}

bool String::empty() const {
    return size_ == 0;
}

const char* String::c_str() const {
    return data_ ? data_ : "";
}

void String::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }

    const std::size_t cap = grown_capacity(capacity_, new_capacity);

    char* new_data = new char[cap + 1];
    if (data_ && size_ != 0) {
        std::memcpy(new_data, data_, size_);
    }
    new_data[size_] = '\0';

    delete[] data_;
    data_ = new_data;
    capacity_ = cap;
}

void String::push_back(char c) {
    if (size_ == capacity_) {
        reserve(size_ + 1);
    }
    data_[size_] = c;
    ++size_;
    data_[size_] = '\0';
}

void String::append(const char* s) {
    if (!s || *s == '\0') {
        return;
    }

    append_bytes(s, std::strlen(s));
}

void String::append(const String& s) {
    append_bytes(s.data_, s.size_);
}

void String::append_bytes(const char* s, std::size_t len) {
    if (len == 0) {
        return;
    }
    if (len > max_capacity - size_) {
        throw std::length_error("String size is too large");
    }

    const std::size_t new_size = size_ + len;
    if (new_size > capacity_) {
        const std::size_t cap = grown_capacity(capacity_, new_size);
        char* new_data = new char[cap + 1];
        if (size_ != 0) {
            std::memcpy(new_data, data_, size_);
        }
        std::memcpy(new_data + size_, s, len);
        delete[] data_;
        data_ = new_data;
        capacity_ = cap;
    } else {
        std::memmove(data_ + size_, s, len);
    }
    size_ = new_size;
    data_[size_] = '\0';
}

void String::print(std::ostream& os) const {
    os.write(c_str(), static_cast<std::streamsize>(size_));
}

std::size_t String::find(const char* substr) const {
    if (!substr || *substr == '\0') {
        return 0;
    }

    const std::size_t len = std::strlen(substr);
    if (len > size_) {
        return npos;
    }

    for (std::size_t i = 0; i + len <= size_; ++i) {
        if (std::memcmp(data_ + i, substr, len) == 0) {
            return i;
        }
    }
    return npos;
}

std::size_t String::count_char(char c) const {
    std::size_t count = 0;
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == c) {
            ++count;
        }
    }
    return count;
}
