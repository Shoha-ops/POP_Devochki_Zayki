#ifndef CORE_CHECK_H
#define CORE_CHECK_H

#include <iostream>
#include <limits>

template<typename T>
T safeInput() {
    T value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a valid value: ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard any extra input
            return value; // Return the valid input
        }
    }
}

#endif // CORE_CHECK_H