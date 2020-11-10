//
//  linked_list_tests.cpp
//  unit_tests.common
//
//  Created by 杨丰 on 2020/11/4.
//

#include <gtest/gtest.h>
#include "../src.algos/linked_list.hpp"

class LinkedListTest : public testing::Test{
public:
    LinkedList<int> buildList() {
        auto list = LinkedList<int>();
        for (int number : numbers) {
            list.append(number);
        }
        return list;
    }
    
    std::vector<int> numbers = {8, 2, 10, 9, 7, 5};
};

TEST_F(LinkedListTest, testEmptyList) {
    auto list = LinkedList<int>();
    ASSERT_TRUE(list.isEmpty());
    EXPECT_EQ(list.count(), 0);
    ASSERT_TRUE(list.first() == nullptr);
    ASSERT_TRUE(list.last() == nullptr);
}
