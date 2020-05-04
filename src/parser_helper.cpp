#include "parser_helper.h"

std::unique_ptr<fd::exp::Expression> ph::uniquePtr(fd::exp::Expression* expression) {
    return std::unique_ptr<fd::exp::Expression>(expression);;
}
