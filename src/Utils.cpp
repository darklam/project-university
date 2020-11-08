#include "Utils.hpp"

#include <stdio.h>

#include <cstring>
#include <sstream>
#include <string>

#include "CustomVector.hpp"

CustomVector<std::string>* Utils::splitString(const std::string& str,
                                      const std::string& delimiter) {
    auto vec = new CustomVector<std::string>(5);
    std::size_t current, previous = 0;
    current = str.find(delimiter);
    while (current != std::string::npos) {
        vec->add(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delimiter, previous);
    }
    vec->add(str.substr(previous, current - previous));

    return vec;
}