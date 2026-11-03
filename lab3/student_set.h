#ifndef STUDENT_SET_H
#define STUDENT_SET_H

#include "student.h"

#include <cstddef>

class StudentSet {
public:
    StudentSet();
    StudentSet(const StudentSet& other);
    StudentSet(StudentSet&& other) noexcept;
    StudentSet& operator=(const StudentSet& other);
    StudentSet& operator=(StudentSet&& other) noexcept;
    ~StudentSet();

    bool add(const Student& student);
    bool remove(const Student& student);
    bool contains(const Student& student) const;
    bool operator()(const Student& student) const;

    std::size_t size() const;
    bool empty() const;
    void swap(StudentSet& other) noexcept;

    StudentSet& operator+=(const Student& student);
    StudentSet& operator-=(const Student& student);
    StudentSet operator|(const StudentSet& other) const;
    StudentSet operator&(const StudentSet& other) const;
    StudentSet operator-(const StudentSet& other) const;
    bool operator==(const StudentSet& other) const;
    bool operator!=(const StudentSet& other) const;

    friend std::ostream& operator<<(std::ostream& os, const StudentSet& set);

private:
    void grow();

    Student* data_;
    std::size_t size_;
    std::size_t capacity_;
};

#endif
