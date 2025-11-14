#pragma once

#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>
#include <stdexcept>

template <typename T>
class List {
    struct Node {
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
    };
    Node* first;
    Node* last;
    size_t size_;
public:
    List() : first(nullptr), last(nullptr), size_(0) {}

    List(const List& other) : first(nullptr), last(nullptr), size_(0) {
        Node* p = other.first;
        while (p != nullptr) {
            push_back(p->data);
            p = p->next;
        }
    }
    List& operator=(const List& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        Node* p = other.first;
        while (p != nullptr) {
            push_back(p->data);
            p = p->next;
        }
        return *this;
    }

    void clear() {
        while (!is_empty()) {
            pop_front();
        }
    }

    ~List() { clear(); }

    bool is_empty() const noexcept { return first == nullptr; }
    size_t size() const noexcept { return size_; }
    T& front() {
        if (is_empty()) { throw std::runtime_error("List is empty"); }
        return first->data;
    }
    T& back() {
        if (is_empty()) { throw std::runtime_error("List is empty"); }
        return last->data;
    }
    const T& front() const {
        if (is_empty()) { throw std::runtime_error("List is empty"); }
        return first->data;
    }
    const T& back() const {
        if (is_empty()) { throw std::runtime_error("List is empty"); }
        return last->data;
    }

    T& get(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node* p = first;
        for (size_t i = 0; i < index; ++i) {
            p = p->next;
        }
        return p->data;
    }
    const T& get(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node* p = first;
        for (size_t i = 0; i < index; ++i) {
            p = p->next;
        }
        return p->data;
    }

    void push_back(const T& data) {
        Node* p = new Node(data);
        if (is_empty()) {
            first = p;
            last = p;
        }
        else {
            last->next = p;
            last = p;
        }
        ++size_;
    }
    void push_front(const T& data) {
        Node* p = new Node(data);
        if (is_empty()) {
            first = p;
            last = p;
        }
        else {
            p->next = first;
            first = p;
        }
        ++size_;
    }
    void pop_front() {
        if (is_empty()) {
            return;
        }
        Node* p = first;
        first = first->next;
        if (is_empty()) {
            last = nullptr;
        }
        delete p;
        --size_;
    }
    void pop_back() {
        if (is_empty()) {
            return;
        }
        if (first == last) {
            delete first;
            first = nullptr;
            last = nullptr;
        }
        else {
            Node* p = first;
            while (p->next != last) {
                p = p->next;
            }
            delete last;
            last = p;
            last->next = nullptr;
        }
        --size_;
    }
};

template <typename T>
class Stack {
    List<T> stack;
public:
    T& top()                     { return stack.front();    }
    const T& top() const         { return stack.front();    }
    bool empty() const noexcept  { return stack.is_empty(); }
    size_t size() const noexcept { return stack.size();     }
    void push(const T& data)     { stack.push_front(data);  }
    void pop()                   { stack.pop_front();       }
    void clear()                 { stack.clear();           }
};

#endif
