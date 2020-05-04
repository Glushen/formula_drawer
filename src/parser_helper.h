#pragma once
#include <utility>
#include <memory>
#include <vector>

namespace ph {
    template<typename T>
    T unwrap(T* ptr) {
        T value = std::move(*ptr);
        delete ptr;
        return value;
    }
}
