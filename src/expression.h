#pragma once

#include <string>
#include <memory>
#include <vector>
#include "view.h"

namespace fd::exp {
    class Expression {
    public:
        virtual std::unique_ptr<fd::v::View> createView() = 0;
        virtual ~Expression() = default;
    };

    class Primitive : public Expression {
    public:
        std::string text;

        explicit Primitive(std::string text);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Bracketed : public Expression {
    public:
        std::unique_ptr<Expression> expression;

        explicit Bracketed(std::unique_ptr<Expression> expression);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Power : public Expression {
    public:
        std::unique_ptr<Expression> base;
        std::unique_ptr<Expression> power;

        Power(std::unique_ptr<Expression> base, std::unique_ptr<Expression> power);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Index : public Expression {
    public:
        std::unique_ptr<Expression> base;
        std::unique_ptr<Expression> index;

        Index(std::unique_ptr<Expression> base, std::unique_ptr<Expression> index);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Unary : public Expression {
    public:
        std::string sign;
        std::unique_ptr<Expression> base;

        Unary(std::string sign, std::unique_ptr<Expression> base);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Binary : public Expression {
    public:
        std::string sign;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;

        Binary(std::string sign, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Division : public Expression {
    public:
        std::unique_ptr<Expression> top;
        std::unique_ptr<Expression> bottom;

        Division(std::unique_ptr<Expression> top, std::unique_ptr<Expression> bottom);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Variadic : public Expression {
    public:
        std::string sign;
        std::unique_ptr<Expression> from;
        std::unique_ptr<Expression> to;
        std::unique_ptr<Expression> body;

        Variadic(std::string sign, std::unique_ptr<Expression> from, std::unique_ptr<Expression> to, std::unique_ptr<Expression> body);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Case {
    public:
        std::unique_ptr<Expression> body;
        std::unique_ptr<Expression> condition;

        Case(std::unique_ptr<Expression> body, std::unique_ptr<Expression> condition);
    };

    class Cases : public Expression {
    public:
        std::vector<Case> cases;

        explicit Cases(std::vector<Case> cases);

        std::unique_ptr<fd::v::View> createView() override;
    };

    class Matrix : public Expression {
    public:
        std::vector<std::vector<std::unique_ptr<Expression>>> matrix;

        explicit Matrix(std::vector<std::vector<std::unique_ptr<Expression>>> matrix);

        void checkCorrectness();

        std::unique_ptr<fd::v::View> createView() override;
    };
}
