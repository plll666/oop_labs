#ifndef OOP_LAB2_STRING_H
#define OOP_LAB2_STRING_H

#include <cstddef>
#include <iosfwd>

class String {
public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    String();
    String(const char* s);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    String& operator=(String other);
    void swap(String& other) noexcept;

    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    const char* c_str() const;

    void reserve(std::size_t new_capacity);

    void push_back(char c);
    void append(const char* s);
    void append(const String& s);
    void print(std::ostream& os) const;

    std::size_t find(const char* substr) const;

    std::size_t count_char(char c) const;

private:
    void append_bytes(const char* s, std::size_t len);

    char* data_;
    std::size_t size_;
    std::size_t capacity_;
};

#endif
