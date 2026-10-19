#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <limits>

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
}

inline void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline int getValidInt(const std::string& prompt, int minVal = 0, int maxVal = 10000) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= minVal && val <= maxVal) {
            return val;
        }
        if (std::cin.eof()) {
            exit(0);
        }
        std::cout << Color::RED << "  [!] Invalid input. Please enter a number between "
                  << minVal << " and " << maxVal << "." << Color::RESET << "\n";
        clearInput();
    }
}

inline void printBanner(const std::string& title) {
    std::cout << "\n" << Color::CYAN << Color::BOLD
              << "==========================================================================\n"
              << "  " << title << "\n"
              << "=========================================================================="
              << Color::RESET << "\n";
}

inline void printSubHeader(const std::string& title) {
    std::cout << "\n" << Color::YELLOW << Color::BOLD << "--- " << title << " ---" << Color::RESET << "\n";
}

#endif // UTILS_HPP
