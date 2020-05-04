#pragma once

#include <string>

namespace fd {
    struct Result {
        bool accepted = false;
        std::string errorMessage;
    };
    Result drawExpression(const std::string& inputExpression, const std::string& outputFileName);
}
