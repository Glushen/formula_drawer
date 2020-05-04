#pragma once
#include <utility>
#include <memory>
#include <vector>
#include "expression.h"

namespace ph {
    template<typename T>
    T unwrap(T* ptr) {
        T value = std::move(*ptr);
        delete ptr;
        return value;
    }

    std::unique_ptr<fd::exp::Expression> uniquePtr(fd::exp::Expression* expression);
}
