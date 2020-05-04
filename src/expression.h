#pragma once

#include <string>

namespace fd::exp {
    class Expression {
    public:
        virtual ~Expression() = default;
    };

    class Primitive : public Expression {
    public:
        std::string data;

        explicit Primitive(std::string data);
    };
}
