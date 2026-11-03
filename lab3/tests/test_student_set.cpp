#include "student_set.h"

#include <gtest/gtest.h>

#include <sstream>
#include <utility>

TEST(StudentTest, Default) {
    Student student;
    EXPECT_STREQ(student.name(), "");
    EXPECT_EQ(student.id(), 0);
}

TEST(StudentTest, EqualityById) {
    Student a("Ivan", 1);
    EXPECT_TRUE(a == Student("Petr", 1));
    EXPECT_FALSE(a == Student("Ivan", 2));
    std::ostringstream os;
    os << a;
    EXPECT_EQ(os.str(), "(1, Ivan)");
}

TEST(StudentTest, CopyConstructor) {
    char name[] = "Ivan";
    Student a(name, 1);
    name[0] = 'X';
    EXPECT_STREQ(a.name(), "Ivan");
    Student b(a);
    EXPECT_NE(a.name(), b.name());
    a = Student("Anna", 2);
    EXPECT_STREQ(b.name(), "Ivan");
    EXPECT_EQ(b.id(), 1);
}

TEST(StudentTest, CopyAssignment) {
    Student a("Ivan", 1);
    Student b("Anna", 2);
    b = a;
    EXPECT_NE(a.name(), b.name());
    a = Student("Oleg", 3);
    b = b;
    EXPECT_STREQ(b.name(), "Ivan");
    EXPECT_EQ(b.id(), 1);
}

TEST(StudentTest, MoveConstructor) {
    Student a("Ivan", 1);
    const char* name = a.name();
    Student b(std::move(a));
    EXPECT_EQ(b.name(), name);
    EXPECT_EQ(b.id(), 1);
    EXPECT_STREQ(a.name(), "");
    EXPECT_EQ(a.id(), 0);
    a = Student("Anna", 2);
    EXPECT_STREQ(a.name(), "Anna");
}

TEST(StudentTest, MoveAssignment) {
    Student a("Ivan", 1);
    Student b("Anna", 2);
    const char* name = a.name();
    b = std::move(a);
    EXPECT_EQ(b.name(), name);
    EXPECT_EQ(b.id(), 1);
    EXPECT_STREQ(a.name(), "");
    EXPECT_EQ(a.id(), 0);
    Student& same = b;
    b = std::move(same);
    EXPECT_STREQ(b.name(), "Ivan");
}

TEST(StudentSetTest, Empty) {
    StudentSet set;
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0u);
    EXPECT_FALSE(set.contains(Student("Ivan", 1)));
    EXPECT_FALSE(set.remove(Student("Ivan", 1)));
}

TEST(StudentSetTest, Add) {
    StudentSet set;
    EXPECT_TRUE(set.add(Student("Ivan", 1)));
    EXPECT_FALSE(set.add(Student("Petr", 1)));
    EXPECT_TRUE(set.add(Student("Ivan", 2)));
    EXPECT_EQ(set.size(), 2u);
    std::ostringstream os;
    os << set;
    EXPECT_EQ(os.str(), "{ (1, Ivan), (2, Ivan) }");
}

TEST(StudentSetTest, Remove) {
    StudentSet set;
    set += Student("Ivan", 1);
    set += Student("Anna", 2);
    set += Student("Oleg", 3);
    EXPECT_TRUE(set.remove(Student("", 2)));
    EXPECT_FALSE(set.remove(Student("", 2)));
    std::ostringstream os;
    os << set;
    EXPECT_EQ(os.str(), "{ (1, Ivan), (3, Oleg) }");
    EXPECT_TRUE(set.remove(Student("", 1)));
    EXPECT_TRUE(set.remove(Student("", 3)));
    EXPECT_TRUE(set.empty());
    EXPECT_TRUE(set.add(Student("Petr", 4)));
    EXPECT_EQ(set.size(), 1u);
}

TEST(StudentSetTest, Contains) {
    StudentSet set;
    set += Student("Ivan", 1);
    EXPECT_TRUE(set.contains(Student("", 1)));
    EXPECT_TRUE(set(Student("", 1)));
    EXPECT_FALSE(set.contains(Student("", 2)));
    EXPECT_FALSE(set(Student("", 2)));
}

TEST(StudentSetTest, CopyConstructor) {
    StudentSet a;
    a += Student("Ivan", 1);
    a += Student("Anna", 2);
    StudentSet b(a);
    a -= Student("", 1);
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(b(Student("", 1)));
    b += Student("Oleg", 3);
    EXPECT_FALSE(a(Student("", 3)));
    std::ostringstream os;
    os << b;
    EXPECT_EQ(os.str(), "{ (1, Ivan), (2, Anna), (3, Oleg) }");
}

TEST(StudentSetTest, CopyAssignment) {
    StudentSet a;
    a += Student("Ivan", 1);
    StudentSet b;
    b += Student("Anna", 2);
    b = a;
    a -= Student("", 1);
    b = b;
    EXPECT_EQ(b.size(), 1u);
    EXPECT_TRUE(b(Student("", 1)));
    EXPECT_FALSE(b(Student("", 2)));
    std::ostringstream os;
    os << b;
    EXPECT_EQ(os.str(), "{ (1, Ivan) }");
}

TEST(StudentSetTest, MoveConstructor) {
    StudentSet a;
    a += Student("Ivan", 1);
    StudentSet b(std::move(a));
    EXPECT_TRUE(b(Student("", 1)));
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0u);
    a += Student("Anna", 2);
    EXPECT_TRUE(a(Student("", 2)));
    EXPECT_FALSE(b(Student("", 2)));
}

TEST(StudentSetTest, MoveAssignment) {
    StudentSet a;
    a += Student("Ivan", 1);
    StudentSet b;
    b += Student("Anna", 2);
    b = std::move(a);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.size(), 1u);
    EXPECT_TRUE(b(Student("", 1)));
    EXPECT_FALSE(b(Student("", 2)));
    StudentSet& same = b;
    b = std::move(same);
    EXPECT_TRUE(b(Student("", 1)));
    a += Student("Oleg", 3);
    EXPECT_TRUE(a(Student("", 3)));
}

TEST(StudentSetTest, AddRemoveOperators) {
    StudentSet set;
    EXPECT_EQ(&(set += Student("Ivan", 1)), &set);
    (set += Student("Anna", 2)) += Student("Oleg", 3);
    EXPECT_EQ(&(set -= Student("", 1)), &set);
    (set -= Student("", 2)) -= Student("", 3);
    EXPECT_TRUE(set.empty());
}

TEST(StudentSetTest, Union) {
    StudentSet a;
    a += Student("Ivan", 1);
    a += Student("Anna", 2);
    StudentSet b;
    b += Student("Petr", 2);
    b += Student("Oleg", 3);
    StudentSet result = a | b;
    EXPECT_EQ(result.size(), 3u);
    std::ostringstream os;
    os << result;
    EXPECT_EQ(os.str(), "{ (1, Ivan), (2, Anna), (3, Oleg) }");
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 2u);
}

TEST(StudentSetTest, Intersection) {
    StudentSet a;
    a += Student("Ivan", 1);
    a += Student("Anna", 2);
    StudentSet b;
    b += Student("Petr", 2);
    b += Student("Oleg", 3);
    StudentSet result = a & b;
    EXPECT_EQ(result.size(), 1u);
    EXPECT_TRUE(result(Student("", 2)));
    std::ostringstream os;
    os << result;
    EXPECT_EQ(os.str(), "{ (2, Anna) }");
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 2u);
}

TEST(StudentSetTest, Difference) {
    StudentSet a;
    a += Student("Ivan", 1);
    a += Student("Anna", 2);
    StudentSet b;
    b += Student("Petr", 2);
    b += Student("Oleg", 3);
    StudentSet result = a - b;
    EXPECT_EQ(result.size(), 1u);
    EXPECT_TRUE(result(Student("", 1)));
    EXPECT_FALSE(result(Student("", 2)));
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 2u);
}

TEST(StudentSetTest, Equality) {
    StudentSet a;
    a += Student("Ivan", 1);
    a += Student("Anna", 2);
    StudentSet b;
    b += Student("Petr", 2);
    b += Student("Oleg", 1);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    b -= Student("", 1);
    b += Student("Ivan", 3);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    b -= Student("", 3);
    EXPECT_FALSE(a == b);
}

TEST(StudentSetTest, Output) {
    StudentSet set;
    std::ostringstream empty;
    empty << set;
    EXPECT_EQ(empty.str(), "{}");
    set += Student("Anna", 2);
    set += Student("Ivan", 1);
    std::ostringstream os;
    os << set;
    EXPECT_EQ(os.str(), "{ (2, Anna), (1, Ivan) }");
}

TEST(StudentSetTest, Stress) {
    StudentSet set;
    for (int i = 0; i < 1000; ++i) {
        ASSERT_TRUE(set.add(Student("Student", i)));
    }
    EXPECT_EQ(set.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(set(Student("", i)));
        EXPECT_FALSE(set.add(Student("Duplicate", i)));
    }
    EXPECT_EQ(set.size(), 1000u);
}

TEST(StudentSetTest, EmptyAndSelfOperations) {
    StudentSet empty;
    StudentSet copied(empty);
    StudentSet moved(std::move(copied));
    EXPECT_TRUE(moved == empty);
    EXPECT_TRUE(copied.empty());
    StudentSet set;
    set += Student("Ivan", 1);
    EXPECT_TRUE((set | empty) == set);
    EXPECT_TRUE((empty | set) == set);
    EXPECT_TRUE((set & empty).empty());
    EXPECT_TRUE((empty & set).empty());
    EXPECT_TRUE((set - empty) == set);
    EXPECT_TRUE((empty - set).empty());
    EXPECT_TRUE((set | set) == set);
    EXPECT_TRUE((set & set) == set);
    EXPECT_TRUE((set - set).empty());
    StudentSet assigned;
    assigned = empty;
    EXPECT_TRUE(assigned.empty());
    set = std::move(empty);
    EXPECT_TRUE(set.empty());
    EXPECT_TRUE(empty.empty());
}
