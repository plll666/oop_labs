#include "lab_string.h"

#include <gtest/gtest.h>

#include <cstring>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

TEST(StringTest, DefaultConstructorIsEmpty) {
    String s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
    EXPECT_EQ(s.capacity(), 1u);
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringTest, ConstructFromLiteral) {
    String s("Hello");
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.size(), 5u);
    EXPECT_STREQ(s.c_str(), "Hello");
}

TEST(StringTest, PushBackGrowsAndKeepsContent) {
    String s;
    s.push_back('a');
    EXPECT_EQ(s.size(), 1u);
    EXPECT_EQ(s.capacity(), 1u);

    s.push_back('b');
    EXPECT_EQ(s.capacity(), 2u);

    s.push_back('c');
    EXPECT_EQ(s.capacity(), 4u);

    s.push_back('d');
    s.push_back('e');
    EXPECT_EQ(s.capacity(), 8u);

    EXPECT_STREQ(s.c_str(), "abcde");
    EXPECT_EQ(s.size(), 5u);
}

TEST(StringTest, AppendCharPtr) {
    String s("Hello");
    s.append(", World!");
    EXPECT_STREQ(s.c_str(), "Hello, World!");
    EXPECT_EQ(s.size(), 13u);
}

TEST(StringTest, AppendString) {
    String s("foo");
    String tail("bar");
    s.append(tail);
    EXPECT_STREQ(s.c_str(), "foobar");
    EXPECT_STREQ(tail.c_str(), "bar");
}

TEST(StringTest, AppendEmptyDoesNothing) {
    String s("abc");
    s.append("");
    s.append(String());
    EXPECT_STREQ(s.c_str(), "abc");
    EXPECT_EQ(s.size(), 3u);
}

TEST(StringTest, AppendOwnBufferAndSuffixWithAndWithoutGrowth) {
    for (bool reserve_first : {false, true}) {
        for (std::size_t offset : {0u, 1u}) {
            String s("abc");
            if (reserve_first) {
                s.reserve(16);
            }
            s.append(s.c_str() + offset);
            EXPECT_STREQ(s.c_str(), offset == 0 ? "abcabc" : "abcbc");
            EXPECT_EQ(s.size(), 6u - offset);
        }
    }
}

TEST(StringTest, AppendStringPreservesEmbeddedNulls) {
    String source("a");
    source.push_back('\0');
    source.push_back('b');
    for (bool reserve_first : {false, true}) {
        String target("x");
        if (reserve_first) {
            target.reserve(16);
        }
        target.append(source);
        ASSERT_EQ(target.size(), 4u);
        EXPECT_EQ(std::memcmp(target.c_str(), "xa\0b\0", 5), 0);
    }
}

TEST(StringTest, SelfAppendPreservesEmbeddedNulls) {
    for (bool reserve_first : {false, true}) {
        String s("a");
        s.push_back('\0');
        s.push_back('b');
        if (reserve_first) {
            s.reserve(16);
        }
        s.append(s);
        ASSERT_EQ(s.size(), 6u);
        EXPECT_EQ(std::memcmp(s.c_str(), "a\0ba\0b\0", 7), 0);
    }
}

TEST(StringTest, ReserveRejectsOverflowAndPreservesString) {
    String s("abc");
    const char* buffer = s.c_str();
    const std::size_t capacity = s.capacity();
    EXPECT_THROW(s.reserve(std::numeric_limits<std::size_t>::max()), std::length_error);
    EXPECT_EQ(s.c_str(), buffer);
    EXPECT_EQ(s.capacity(), capacity);
    EXPECT_EQ(s.size(), 3u);
    EXPECT_STREQ(s.c_str(), "abc");
    s.append("d");
    EXPECT_STREQ(s.c_str(), "abcd");
}

TEST(StringTest, FindReturnsPosition) {
    String s("hello world");
    EXPECT_EQ(s.find("world"), 6u);
    EXPECT_EQ(s.find("hello"), 0u);
    EXPECT_EQ(s.find(""), 0u);
    EXPECT_EQ(s.find("hello world"), 0u);
}

TEST(StringTest, FindReturnsNposWhenAbsent) {
    String s("hello");
    EXPECT_EQ(s.find("x"), String::npos);
    EXPECT_EQ(s.find("hello!"), String::npos);
    EXPECT_EQ(s.find("world"), String::npos);
}

TEST(StringTest, PrintWritesToStream) {
    String s("print me");
    std::ostringstream os;
    s.print(os);
    EXPECT_EQ(os.str(), "print me");
}

TEST(StringTest, CopyConstructorMakesDeepCopy) {
    String original("abc");
    String copy(original);
    original.append("d");

    EXPECT_STREQ(copy.c_str(), "abc");
    EXPECT_STREQ(original.c_str(), "abcd");
    EXPECT_NE(copy.c_str(), original.c_str());
}

TEST(StringTest, CopyAssignmentMakesDeepCopy) {
    String source("hello");
    String target("x");
    target = source;
    source.append("!");

    EXPECT_STREQ(target.c_str(), "hello");
    EXPECT_STREQ(source.c_str(), "hello!");
    EXPECT_NE(target.c_str(), source.c_str());
}

TEST(StringTest, SelfAssignmentIsSafe) {
    String s("self");
    s = s;
    EXPECT_STREQ(s.c_str(), "self");
    EXPECT_EQ(s.size(), 4u);
}

TEST(StringTest, MoveConstructorStealsBufferAndEmptiesSource) {
    String source("move me");
    String moved(std::move(source));

    EXPECT_STREQ(moved.c_str(), "move me");
    EXPECT_EQ(moved.size(), 7u);

    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.size(), 0u);
    EXPECT_STREQ(source.c_str(), "");
}

TEST(StringTest, MoveAssignmentStealsBufferAndEmptiesSource) {
    String source("moved");
    String target("old");
    target = std::move(source);

    EXPECT_STREQ(target.c_str(), "moved");
    EXPECT_TRUE(source.empty());
    EXPECT_STREQ(source.c_str(), "");
}

TEST(StringTest, CountCharFindsAllOccurrences) {
    String s("hello");
    EXPECT_EQ(s.count_char('l'), 2u);
    EXPECT_EQ(s.count_char('o'), 1u);
    EXPECT_EQ(s.count_char('h'), 1u);
    EXPECT_EQ(s.count_char('z'), 0u);
}

TEST(StringTest, CountCharOnEmptyString) {
    String s;
    EXPECT_EQ(s.count_char('a'), 0u);
}

TEST(StringTest, CountCharIgnoresTerminator) {
    String s("a");
    EXPECT_EQ(s.count_char('\0'), 0u);
}

TEST(StringTest, SingleCharacterString) {
    String s("x");
    EXPECT_EQ(s.size(), 1u);
    EXPECT_EQ(s.count_char('x'), 1u);
    EXPECT_EQ(s.find("x"), 0u);
}
