//
//  constants.h
//  DigitalRender
//
//  Created by 杨丰 on 2020/11/3.
//

#ifndef constants_h
#define constants_h

#include <cmath>
#include <limits>
#include <vector>

// MARK: Zero

//! Zero size_t.
constexpr size_t kZeroSize = 0;

//! Zero ssize_t.
constexpr ssize_t kZeroSSize = 0;

//! Zero for type T.
template <typename T>
constexpr T zero() {
    return static_cast<T>(0);
}

//! Zero for float.
template <>
constexpr float zero<float>() {
    return 0.f;
}

//! Zero for double.
template <>
constexpr double zero<double>() {
    return 0.0;
}


#endif /* constants_h */
