#include "Utils.hpp"

#include <stdio.h>

#include <cstring>
#include <sstream>
#include <string>

#include "List.hpp"

List<std::string>* Utils::splitString(std::string str,
                                      std::string delimiter) {
    auto list = new List<std::string>();
    std::size_t current, previous = 0;
    current = str.find(delimiter);
    while (current != std::string::npos) {
        list->add(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delimiter, previous);
    }
    list->add(str.substr(previous, current - previous));

    return list;
}