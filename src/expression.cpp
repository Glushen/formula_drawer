#include "expression.h"
#include <utility>

fd::exp::Primitive::Primitive(std::string text): text(std::move(text)) { }

std::unique_ptr<fd::v::View> fd::exp::Primitive::createView() {
    return std::make_unique<fd::v::TextView>(text);
}

fd::exp::Bracketed::Bracketed(std::unique_ptr<Expression> expression): expression(std::move(expression)) { }

std::unique_ptr<fd::v::View> fd::exp::Bracketed::createView() {
    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(std::make_unique<fd::v::OpeningRoundBracketView>());
    elements.push_back(expression->createView());
    elements.push_back(std::make_unique<fd::v::ClosingRoundBracketView>());
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Power::Power(std::unique_ptr<Expression> base, std::unique_ptr<Expression> power):
    base(std::move(base)), power(std::move(power)) { }

std::unique_ptr<fd::v::View> fd::exp::Power::createView() {
    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(base->createView());
    elements.push_back(std::make_unique<fd::v::TranslateLayout>(std::make_unique<fd::v::ScaleLayout>(power->createView(), 0.5), -10));
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Index::Index(std::unique_ptr<Expression> base, std::unique_ptr<Expression> index):
    base(std::move(base)), index(std::move(index)) { }

std::unique_ptr<fd::v::View> fd::exp::Index::createView() {
    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(base->createView());
    elements.push_back(std::make_unique<fd::v::TranslateLayout>(std::make_unique<fd::v::ScaleLayout>(index->createView(), 0.5), 10));
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Unary::Unary(std::string sign, std::unique_ptr<Expression> base):
    sign(std::move(sign)), base(std::move(base)) { }

std::unique_ptr<fd::v::View> fd::exp::Unary::createView() {
    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(std::make_unique<fd::v::TextView>(sign));
    elements.push_back(base->createView());
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Binary::Binary(std::string sign, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right):
    sign(std::move(sign)), left(std::move(left)), right(std::move(right)) { }

std::unique_ptr<fd::v::View> fd::exp::Binary::createView() {
    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(left->createView());
    elements.push_back(std::make_unique<fd::v::TextView>(sign));
    elements.push_back(right->createView());
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Division::Division(std::unique_ptr<Expression> top, std::unique_ptr<Expression> bottom):
    top(std::move(top)), bottom(std::move(bottom)) { }

std::unique_ptr<fd::v::View> fd::exp::Division::createView() {
    return std::make_unique<fd::v::FractionLayout>(top->createView(), bottom->createView());
}

fd::exp::Variadic::Variadic(std::string sign, std::unique_ptr<Expression> from, std::unique_ptr<Expression> to, std::unique_ptr<Expression> body):
    sign(std::move(sign)), from(std::move(from)), to(std::move(to)), body(std::move(body)) { }

std::unique_ptr<fd::v::View> fd::exp::Variadic::createView() {
    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(std::make_unique<fd::v::TripleVerticalLayout>(
        to->createView(),
        std::make_unique<fd::v::TextView>(sign),
        from->createView()
    ));
    elements.push_back(body->createView());
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Case::Case(std::unique_ptr<Expression> body, std::unique_ptr<Expression> condition):
    body(std::move(body)), condition(std::move(condition)) { }

fd::exp::Cases::Cases(std::vector<Case> cases): cases(std::move(cases)) { }

std::unique_ptr<fd::v::View> fd::exp::Cases::createView() {
    auto rows = std::vector<std::vector<std::unique_ptr<fd::v::View>>>();

    for (auto& aCase : cases) {
        auto elements = std::vector<std::unique_ptr<fd::v::View>>();
        elements.push_back(aCase.body->createView());
        elements.push_back(aCase.condition->createView());
        rows.push_back(std::move(elements));
    }

    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(std::make_unique<fd::v::OpeningCurlyBracketView>());
    elements.push_back(std::make_unique<fd::v::GridLayout>(std::move(rows)));
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}

fd::exp::Matrix::Matrix(std::vector<std::vector<std::unique_ptr<Expression>>> matrix): matrix(std::move(matrix)) { }

void fd::exp::Matrix::checkCorrectness() {
    for (const auto& row : matrix) {
        if (row.size() != matrix[0].size()) {
            throw std::invalid_argument("Matrix should have rows of equal length");
        }
    }
}

std::unique_ptr<fd::v::View> fd::exp::Matrix::createView() {
    auto gridRows = std::vector<std::vector<std::unique_ptr<fd::v::View>>>();

    for (auto& row : matrix) {
        auto gridRow = std::vector<std::unique_ptr<fd::v::View>>();
        for (auto& item : row) {
            gridRow.push_back(item->createView());
        }
        gridRows.push_back(std::move(gridRow));
    }

    auto elements = std::vector<std::unique_ptr<fd::v::View>>();
    elements.push_back(std::make_unique<fd::v::OpeningRoundBracketView>());
    elements.push_back(std::make_unique<fd::v::GridLayout>(std::move(gridRows)));
    elements.push_back(std::make_unique<fd::v::ClosingRoundBracketView>());
    return std::make_unique<fd::v::HorizontalLayout>(std::move(elements));
}
