#pragma once

#include <string>
#include <memory>

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

    class Bracketed : public Expression {
    public:
        std::unique_ptr<Expression> expression;

        explicit Bracketed(std::unique_ptr<Expression> expression);
    };

    class Power : public Expression {
    public:
        std::unique_ptr<Expression> base;
        std::unique_ptr<Expression> power;

        Power(std::unique_ptr<Expression> base, std::unique_ptr<Expression> power);
    };

    class Index : public Expression {
    public:
        std::unique_ptr<Expression> base;
        std::unique_ptr<Expression> index;

        Index(std::unique_ptr<Expression> base, std::unique_ptr<Expression> index);
    };

    class Unary : public Expression {
    public:
        std::string sign;
        std::unique_ptr<Expression> base;

        Unary(std::string sign, std::unique_ptr<Expression> base);
    };

    class Binary : public Expression {
    public:
        std::string sign;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;

        Binary(std::string sign, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    };

    class Division : public Expression {
    public:
        std::unique_ptr<Expression> top;
        std::unique_ptr<Expression> bottom;

        Division(std::unique_ptr<Expression> top, std::unique_ptr<Expression> bottom);
    };

    class Variadic : public Expression {
    public:
        std::string sign;
        std::unique_ptr<Expression> from;
        std::unique_ptr<Expression> to;
        std::unique_ptr<Expression> body;

        Variadic(std::string sign, std::unique_ptr<Expression> from, std::unique_ptr<Expression> to, std::unique_ptr<Expression> body);
    };
}
