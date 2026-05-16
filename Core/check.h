#ifndef CORE_CHECK_H
#define CORE_CHECK_H

#include <iostream>
#include <limits>

template<typename T>
T safeInput(T minValue = std::numeric_limits<T>::lowest(), T maxValue = std::numeric_limits<T>::max()) {
    T value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid value: ";
        } else if (value < minValue || value > maxValue) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Value must be between " << minValue << " and " << maxValue << ". Try again: ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

#endif // CORE_CHECK_H