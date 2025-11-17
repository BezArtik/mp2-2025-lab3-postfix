#include "arithmetic.h"

int main() {
    bool is_continue = true;
    while (is_continue) {
        try {
            input_and_calculate();
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Try again." << std::endl;
            continue;
        }
        std::string sym;
        bool is_continue_2 = true;
        while (is_continue_2) {
            std::cout << "Calculate following expression? (y/n): ";
            std::getline(std::cin, sym);
            if (sym == "y" || sym == "Y") {
                is_continue_2 = false;
                continue;
            }
            else if (sym == "n" || sym == "N") {
                is_continue_2 = false;
                is_continue = false;
            }
            else {
                std::cout << "Try again." << std::endl;
                continue;
            }
        }

    }
    return 0;
}
