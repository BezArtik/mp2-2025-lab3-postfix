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
        std::cout << "Calculate following expression? (y/n): ";
        std::getline(std::cin, sym);
        if (sym == "y" || sym == "Y") {
            continue;
        }
        else {
            is_continue = false;
        }
    }
    return 0;
}
