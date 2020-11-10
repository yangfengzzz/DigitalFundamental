//
//  stack.hpp
//  src.common
//
//  Created by 杨丰 on 2020/11/3.
//

#ifndef stack_hpp
#define stack_hpp

#include <vector>
#include <optional>

template <class T>
class Stack {
public:
    bool isEmpty() {
        return array.empty();
    }
    
    size_t count() {
        return array.size();
    }
    
    void push(T element) {
        array.push_back(element);
    }
    
    std::optional<T> pop() {
        if (!array.empty()) {
            T val = array.back();
            array.pop_back();
            return val;
        }else {
            return {};
        }
    }
    
    std::optional<T> top() {
        if (!array.empty()) {
            return array.back();
        }else {
            return {};
        }
    }
    
private:
    std::vector<T> array;
};

#endif /* stack_hpp */
