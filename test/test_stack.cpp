#include "stack.h"
#include <gtest.h>

TEST(List, can_create_list) {
    ASSERT_NO_THROW(List<int> list);
}

TEST(List, can_create_copied_list) {
    List<int> list;
    ASSERT_NO_THROW(List<int> list1(list));
}

TEST(List, can_assign_list) {
    List<int> list;
    ASSERT_NO_THROW(List<int> list1 = list);
}

TEST(List, list_without_data_is_empty) {
    List<int> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(List, can_push_back_element_in_list) {
    List<int> list;
    ASSERT_NO_THROW(list.push_back(42));
}

TEST(List, can_push_front_element_in_list) {
    List<int> list;
    ASSERT_NO_THROW(list.push_front(42));
}

TEST(List, list_with_data_is_not_empty) {
    List<int> list;
    list.push_back(42);
    EXPECT_FALSE(list.is_empty());
    EXPECT_NE(list.size(), 0);
}

TEST(List, can_get_back_at_the_list) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    EXPECT_EQ(list.back(), 62); 
}

TEST(List, can_get_front_at_the_list) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    EXPECT_EQ(list.front(), 42); 
}

TEST(List, can_get_elements_by_index_0) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    EXPECT_EQ(list.get(0), 42);
}

TEST(List, can_get_elements_by_index_1) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    EXPECT_EQ(list.get(1), 52);
}

TEST(List, can_get_elements_by_index_2) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    EXPECT_EQ(list.get(2), 62);
}

TEST(List, can_not_get_elements_by_incorrect_index) {
    List<int> list;
    list.push_back(42);
    ASSERT_ANY_THROW(list.get(5));
}

TEST(List, can_pop_back_at_the_list) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 52);
}

TEST(List, can_pop_front_at_the_list) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 52);
}

TEST(List, can_clear_the_list) {
    List<int> list;
    list.push_back(42);
    list.push_back(52);
    list.push_back(62);
    ASSERT_NO_THROW(list.clear());
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.is_empty());
}

TEST(List, pop_back_on_empty_list_does_nothing) {
    List<int> list;

    ASSERT_NO_THROW(list.pop_back());
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(List, pop_front_on_empty_list_does_nothing) {
    List<int> list;

    ASSERT_NO_THROW(list.pop_front());
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(List, multiple_pop_back_on_empty_list_does_nothing) {
    List<int> list;

    ASSERT_NO_THROW(list.pop_back());
    ASSERT_NO_THROW(list.pop_back());
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(List, multiple_pop_front_on_empty_list_does_nothing) {
    List<int> list;

    ASSERT_NO_THROW(list.pop_front());
    ASSERT_NO_THROW(list.pop_front());
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(List, front_on_empty_list_throws) {
    List<int> list;
    ASSERT_ANY_THROW(list.front());
}

TEST(List, back_on_empty_list_throws) {
    List<int> list;
    ASSERT_ANY_THROW(list.back());
}

TEST(List, get_on_empty_list_throws) {
    List<int> list;
    ASSERT_ANY_THROW(list.get(0));
}

TEST(List, pop_back_on_single_element_list) {
    List<int> list;
    list.push_back(42);

    list.pop_back();
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
    ASSERT_ANY_THROW(list.front());
    ASSERT_ANY_THROW(list.back());
}

TEST(List, pop_front_on_single_element_list) {
    List<int> list;
    list.push_back(42);

    list.pop_front();
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
    ASSERT_ANY_THROW(list.front());
    ASSERT_ANY_THROW(list.back());
}

TEST(List, pop_back_removes_last_element) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);
    EXPECT_EQ(list.front(), 1);
}

TEST(List, pop_front_removes_first_element) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.back(), 3);
}

TEST(List, large_number_of_operations_list) {
    List<int> list;
    const size_t count = 10000;

    for (size_t i = 0; i < count; ++i) {
        list.push_back(i);
    }
    EXPECT_EQ(list.size(), count);
    EXPECT_EQ(list.back(), count - 1);

    for (size_t i = 0; i < count; ++i) {
        list.pop_front();
    }
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}


TEST(Stack, can_create_stack) {
    ASSERT_NO_THROW(Stack<int> stack);
}

TEST(Stack, can_push_element_in_stack) {
    Stack<int> stack;
    ASSERT_NO_THROW(stack.push(42));
}

TEST(Stack, stack_without_data_is_empty) {
    Stack<int> stack;
    EXPECT_EQ(stack.size(), 0);
    EXPECT_TRUE(stack.empty());
}

TEST(Stack, stack_with_data_is_not_empty) {
    Stack<int> stack;
    stack.push(42);
    EXPECT_EQ(stack.size(), 1);
    EXPECT_FALSE(stack.empty());
}

TEST(Stack, pop_for_single_element_stack) {
    Stack<int> stack;
    stack.push(42);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
    EXPECT_TRUE(stack.empty());
}

TEST(Stack, can_get_top_from_stack) {
    Stack<int> stack;
    stack.push(42);
    ASSERT_NO_THROW(stack.top());
    EXPECT_EQ(stack.top(), 42);
}

TEST(Stack, can_not_get_top_from_empty_stack) {
    Stack<int> stack;
    ASSERT_ANY_THROW(stack.top());
}

TEST(Stack, stack_elements_are_arranged_in_the_correct_order) {
    Stack<int> stack;
    stack.push(42);
    stack.push(52);
    stack.push(62);

    EXPECT_EQ(stack.top(), 62);
    EXPECT_EQ(stack.size(), 3);
    stack.pop();
    EXPECT_EQ(stack.top(), 52);
    EXPECT_EQ(stack.size(), 2);
    stack.pop();
    EXPECT_EQ(stack.top(), 42);
    EXPECT_EQ(stack.size(), 1);
}

TEST(Stack, can_clear_stack) {
    Stack<int> stack;
    stack.push(42);
    stack.push(52);
    stack.push(62);
    
    stack.clear();
    EXPECT_EQ(stack.size(), 0);
    EXPECT_TRUE(stack.empty());
}

TEST(Stack, pop_on_empty_stack_does_nothing) {
    Stack<int> stack;
    ASSERT_NO_THROW(stack.pop());
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}

TEST(Stack, large_number_of_operations_stack) {
    Stack<int> stack;
    const size_t count = 10000;

    for (size_t i = 0; i < count; ++i) {
        stack.push(i);
    }
    EXPECT_EQ(stack.size(), count);
    EXPECT_EQ(stack.top(), count - 1);

    for (size_t i = 0; i < count; ++i) {
        stack.pop();
    }
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}