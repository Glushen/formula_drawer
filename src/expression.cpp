#include "expression.h"
#include <utility>

fd::exp::Primitive::Primitive(std::string data): data(std::move(data)) { }

fd::exp::Bracketed::Bracketed(std::unique_ptr<Expression> expression): expression(std::move(expression)) { }

fd::exp::Power::Power(std::unique_ptr<Expression> base, std::unique_ptr<Expression> power):
    base(std::move(base)), power(std::move(power)) { }

fd::exp::Index::Index(std::unique_ptr<Expression> base, std::unique_ptr<Expression> index):
    base(std::move(base)), index(std::move(index)) { }

fd::exp::Unary::Unary(std::string sign, std::unique_ptr<Expression> base):
    sign(std::move(sign)), base(std::move(base)) { }

fd::exp::Binary::Binary(std::string sign, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right):
    sign(std::move(sign)), left(std::move(left)), right(std::move(right)) { }

fd::exp::Division::Division(std::unique_ptr<Expression> top, std::unique_ptr<Expression> bottom):
    top(std::move(top)), bottom(std::move(bottom)) { }

fd::exp::Variadic::Variadic(std::string sign, std::unique_ptr<Expression> from, std::unique_ptr<Expression> to, std::unique_ptr<Expression> body):
    sign(std::move(sign)), from(std::move(from)), to(std::move(to)), body(std::move(body)) { }

fd::exp::Case::Case(std::unique_ptr<Expression> body, std::unique_ptr<Expression> condition):
    body(std::move(body)), condition(std::move(condition)) { }

fd::exp::Cases::Cases(std::vector<Case> cases): cases(std::move(cases)) { }

fd::exp::Matrix::Matrix(std::vector<std::vector<std::unique_ptr<Expression>>> matrix): matrix(std::move(matrix)) { }

void fd::exp::Matrix::checkCorrectness() {
    for (const auto& row : matrix) {
        if (row.size() != matrix[0].size()) {
            throw std::invalid_argument("Matrix should have rows of equal length");
        }
    }
}
