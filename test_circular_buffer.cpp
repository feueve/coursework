#include "circular_buffer.hpp"
#include "gtest/gtest.h"
#include <fstream>
#include <cstdio>

// ==================== BASIC TESTS ====================

TEST(CircularBufferTest, Constructor) {
CircularBuffer<int> buffer(5);

EXPECT_EQ(buffer.capacity(), 5);
EXPECT_TRUE(buffer.empty());
EXPECT_FALSE(buffer.full());
EXPECT_EQ(buffer.size(), 0);
}

TEST(CircularBufferTest, PushAndSize) {
CircularBuffer<int> buffer(3);

buffer.push(1);
EXPECT_EQ(buffer.size(), 1);
EXPECT_EQ(buffer.front(), 1);

buffer.push(2);
buffer.push(3);
EXPECT_EQ(buffer.size(), 3);
EXPECT_TRUE(buffer.full());
}

TEST(CircularBufferTest, OverflowBehavior) {
CircularBuffer<int> buffer(3);

buffer.push(1);
buffer.push(2);
buffer.push(3);
buffer.push(4); // Should overwrite 1

EXPECT_TRUE(buffer.full());
EXPECT_EQ(buffer.size(), 3);
EXPECT_EQ(buffer.front(), 2);

EXPECT_EQ(buffer[0], 2);
EXPECT_EQ(buffer[1], 3);
EXPECT_EQ(buffer[2], 4);
}

TEST(CircularBufferTest, Pop) {
CircularBuffer<int> buffer(3);

buffer.push(10);
buffer.push(20);
buffer.push(30);

buffer.pop();
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer.front(), 20);

buffer.pop();
EXPECT_EQ(buffer.size(), 1);
EXPECT_EQ(buffer.front(), 30);
}

TEST(CircularBufferTest, RandomAccess) {
CircularBuffer<int> buffer(5);

for (int i = 0; i < 5; ++i) {
buffer.push(i * 10);
}

EXPECT_EQ(buffer[0], 0);
EXPECT_EQ(buffer[2], 20);
EXPECT_EQ(buffer[4], 40);
}

TEST(CircularBufferTest, Clear) {
CircularBuffer<int> buffer(5);

for (int i = 0; i < 5; ++i) {
buffer.push(i);
}

buffer.clear();
EXPECT_TRUE(buffer.empty());
EXPECT_FALSE(buffer.full());
EXPECT_EQ(buffer.size(), 0);
}

TEST(CircularBufferTest, Iterator) {
CircularBuffer<int> buffer(4);

buffer.push(10);
buffer.push(20);
buffer.push(30);

int sum = 0;
for (int val : buffer) {
sum += val;
}
EXPECT_EQ(sum, 60);
}

TEST(CircularBufferTest, CopyConstructor) {
CircularBuffer<int> buffer1(3);
buffer1.push(100);
buffer1.push(200);

CircularBuffer<int> buffer2 = buffer1;

EXPECT_EQ(buffer2.size(), 2);
EXPECT_EQ(buffer2.capacity(), 3);
EXPECT_EQ(buffer2.front(), 100);
EXPECT_EQ(buffer2[1], 200);
}

TEST(CircularBufferTest, MoveConstructor) {
CircularBuffer<int> buffer1(3);
buffer1.push(50);
buffer1.push(60);

CircularBuffer<int> buffer2 = std::move(buffer1);

EXPECT_EQ(buffer2.size(), 2);
EXPECT_EQ(buffer2.front(), 50);
EXPECT_TRUE(buffer1.empty());
}

TEST(CircularBufferTest, Resize) {
CircularBuffer<int> buffer(3);
buffer.push(1);
buffer.push(2);

buffer.resize(5);
EXPECT_EQ(buffer.capacity(), 5);
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer.front(), 1);
EXPECT_EQ(buffer[1], 2);

buffer.resize(2);
EXPECT_EQ(buffer.capacity(), 2);
EXPECT_EQ(buffer.size(), 2);
}

// Тест для emplace с std::pair (вместо Point)
TEST(CircularBufferTest, EmplaceWithPair) {
CircularBuffer<std::pair<int, std::string>> buffer(2);
buffer.emplace(1, "One");
buffer.emplace(2, "Two");

EXPECT_EQ(buffer.front().first, 1);
EXPECT_EQ(buffer.front().second, "One");
EXPECT_EQ(buffer.back().first, 2);
EXPECT_EQ(buffer.back().second, "Two");
}

TEST(CircularBufferTest, FileOperationsBinary) {
CircularBuffer<int> buffer1(5);
for (int i = 1; i <= 5; ++i) {
buffer1.push(i * 10);
}

const std::string filename = "test_binary.bin";
buffer1.saveToFile(filename);

CircularBuffer<int> buffer2(1);
buffer2.loadFromFile(filename);

EXPECT_EQ(buffer2.capacity(), 5);
EXPECT_EQ(buffer2.size(), 5);
EXPECT_EQ(buffer2[0], 10);
EXPECT_EQ(buffer2[4], 50);

std::remove(filename.c_str());
}

TEST(CircularBufferTest, FileOperationsText) {
CircularBuffer<int> buffer1(3);
buffer1.push(100);
buffer1.push(200);
buffer1.push(300);

const std::string filename = "test_text.txt";
buffer1.saveToTextFile(filename);

CircularBuffer<int> buffer2(1);
buffer2.loadFromTextFile(filename);

EXPECT_EQ(buffer2.capacity(), 3);
EXPECT_EQ(buffer2.size(), 3);
EXPECT_EQ(buffer2.front(), 100);
EXPECT_EQ(buffer2.back(), 300);

std::remove(filename.c_str());
}

TEST(CircularBufferTest, EdgeCases) {
// Buffer of size 1
CircularBuffer<int> single_buffer(1);
single_buffer.push(42);
EXPECT_TRUE(single_buffer.full());
EXPECT_EQ(single_buffer.front(), 42);

single_buffer.push(99);
EXPECT_EQ(single_buffer.front(), 99);
}

// ==================== MAIN FOR GOOGLE TEST ====================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}