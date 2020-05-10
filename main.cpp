#include <vector>
#include <string>
#include <iostream>
#include <formula_drawer.h>
#include <QApplication>

int main(int argc, char** argv) {
    QApplication application(argc, argv);
    QApplication::processEvents();

    std::vector<std::string> arguments;
    for (int i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }

    if (arguments.size() % 2 != 0) {
        std::cerr << "Error: Incorrect count of arguments" << std::endl;
        return 1;
    }

    std::string inputExpression, outputFileName;

    bool isCurrentArgumentInput = false;
    for (int i = 0; i < arguments.size(); i += 1) {
        if (i % 2 == 0) {
            if (arguments[i] == "-i") {
                isCurrentArgumentInput = true;
            } else if (arguments[i] == "-o") {
                isCurrentArgumentInput = false;
            } else {
                std::cerr << "Error: Unknown option " << arguments[i] << std::endl;
                return 1;
            }
        } else {
            if (isCurrentArgumentInput) {
                inputExpression = arguments[i];
            } else {
                outputFileName = arguments[i];
            }
        }
    }

    if (inputExpression.empty()) {
        std::cout << "Enter expression: ";
        std::getline(std::cin, inputExpression);
    }
    if (outputFileName.empty()) {
        std::cout << "Enter output file name: ";
        std::getline(std::cin, outputFileName);
    }

    auto result = fd::drawExpression(inputExpression, outputFileName);
    if (result.accepted) {
        std::cout << "Success" << std::endl;
        return 0;
    } else {
        std::cerr << "Error: " << result.errorMessage << std::endl;
        return 1;
    }
}
