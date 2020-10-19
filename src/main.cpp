#include <iostream>
#include "JSONParsing.hpp"
#include "FileSystem.hpp"
#include "List.hpp"

int main() {
    JSONParsing::parseJSON("../datasets/cameras/buy.net/4233.json", "whatever");
    auto result = FileSystem::listContents("../datasets/cameras", 'd');
    auto arr = result->toArray();
    auto length = result->getLength();
    delete result;
    
    for (int i = 0; i < length; i++) {
        auto curr = arr[i];
        if (curr == NULL) {
            printf("Oof\n");
            continue;
        }
        printf("%s\n", curr);
        delete[] curr;
    }
    delete[] arr;
    return 0;
}