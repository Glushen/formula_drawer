#include "expression.h"
#include <utility>

fd::exp::Primitive::Primitive(std::string data): data(std::move(data)) { }
