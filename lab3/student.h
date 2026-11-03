#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>

class Student {
public:
    Student();
    Student(const char* name, int id);
    Student(const Student& other);
    Student(Student&& other) noexcept;
    Student& operator=(const Student& other);
    Student& operator=(Student&& other) noexcept;
    ~Student();

    const char* name() const;
    int id() const;
    void swap(Student& other) noexcept;

    bool operator==(const Student& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Student& student);

private:
    char* name_;
    int id_;
};

#endif
