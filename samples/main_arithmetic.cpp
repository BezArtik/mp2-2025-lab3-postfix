#include "arithmetic_expression/arithmetic.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <exception>
#include <ios>

namespace sample {

void input_and_calculate() {
    std::string expression;
    std::cout << "Enter an expression: ";
    std::getline(std::cin, expression);

    arithmetic::ArithmeticExpression calc(expression);

    if (calc.has_variables()) {
        auto vars = calc.get_variable_names();

        for (const auto& var : vars) {
            std::string input_line;
            double value;
            bool valid_input = false;

            while (!valid_input) {
                std::cout << "Enter a value for " << var << ": ";
                std::getline(std::cin, input_line);

                if (input_line.empty()) {
                    std::cout << "Empty input. Please enter a value." << std::endl;
                    continue;
                }

                try {
                    arithmetic::ArithmeticExpression temp_calc(input_line);

                    if (temp_calc.has_variables()) {
                        std::cout << "Variables inside variable values are not allowed." << std::endl;
                        continue;
                    }

                    value = temp_calc.calculate();
                    valid_input = true;
                } catch (const std::exception& e) {
                    std::cout << "Invalid value: " << e.what() << std::endl;
                }
            }

            calc.set_variable(var, value);
        }

        std::cout << "Result: " << std::fixed << std::setprecision(7)
            << calc.calculate() << std::endl;
    } else {
        std::cout << "Result: " << std::fixed << std::setprecision(7)
            << calc.calculate() << std::endl;
    }
}

bool ask_yes_no() {
    char answer;
    while (true) {
        std::cout << "Calculate another expression? (y/n): ";
        std::cin >> answer;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (answer == 'y' || answer == 'Y') return true;
        if (answer == 'n' || answer == 'N') return false;
        std::cout << "Invalid input. Please enter y or n." << std::endl;
    }
}

}

int main() {
    do {
        try {
            sample::input_and_calculate();
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    } while (sample::ask_yes_no());

    return 0;
}