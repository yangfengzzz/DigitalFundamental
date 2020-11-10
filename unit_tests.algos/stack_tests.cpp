//
//  main.cpp
//  unit_tests.common
//
//  Created by 杨丰 on 2020/11/2.
//

#include <gtest/gtest.h>
#include "../src.algos/stack.hpp"

TEST(StackTest, testEmpty) {
    auto stack = Stack<int>();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 0);
    EXPECT_EQ(stack.top(), std::nullopt);
    ASSERT_TRUE(stack.pop() == std::nullopt);
}

TEST(StackTest, testOneElement) {
    auto stack = Stack<int>();
    
    stack.push(123);
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 1);
    EXPECT_EQ(stack.top(), 123);
    
    const auto result = stack.pop();
    EXPECT_EQ(result, 123);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 0);
    EXPECT_EQ(stack.top(), std::nullopt);
    ASSERT_TRUE(stack.pop() == std::nullopt);
}

TEST(StackTest, testTwoElements) {
    auto stack = Stack<int>();
    
    stack.push(123);
    stack.push(456);
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 2);
    EXPECT_EQ(stack.top(), 456);
    
    const auto result1 = stack.pop();
    EXPECT_EQ(result1, 456);
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 1);
    EXPECT_EQ(stack.top(), 123);
    
    const auto result2 = stack.pop();
    EXPECT_EQ(result2, 123);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 0);
    EXPECT_EQ(stack.top(), std::nullopt);
    ASSERT_TRUE(stack.pop() == std::nullopt);
}

TEST(StackTest, testMakeEmpty) {
    auto stack = Stack<int>();
    
    stack.push(123);
    stack.push(456);
    ASSERT_FALSE(stack.pop() == std::nullopt);
    ASSERT_FALSE(stack.pop() == std::nullopt);
    ASSERT_TRUE(stack.pop() == std::nullopt);
    
    stack.push(789);
    EXPECT_EQ(stack.count(), 1);
    EXPECT_EQ(stack.top(), 789);
    
    const auto result = stack.pop();
    EXPECT_EQ(result, 789);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.count(), 0);
    EXPECT_EQ(stack.top(), std::nullopt);
    ASSERT_TRUE(stack.pop() == std::nullopt);
}
