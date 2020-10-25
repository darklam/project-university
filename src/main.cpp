#include <iostream>
#include "JSONParsing.hpp"
#include "FileSystem.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include "HashMap.hpp"

int main() {
    JSONParsing::parseJSON("../datasets/cameras/buy.net/4233.json", "whatever");
    auto result = FileSystem::listContents("../datasets/cameras", 'd');
    List<char*>* files = new List<char*>();
    for (auto current = result->getRoot(); current != nullptr; current = *(current->getNext())) {
        auto value = current->getValue();
        if (value == NULL) {
            printf("Oof\n");
            continue;
        }
        if (Utils::compareStrings(value, "..") || Utils::compareStrings(value, ".")) {
            continue;
        }
        auto path = FileSystem::join("../datasets/cameras", value);
        auto currentFiles = FileSystem::listContents(path, 'f');
        delete[] path;
        for (auto j = currentFiles->getRoot(); j != nullptr; j = *(j->getNext())) {
            auto val = j->getValue();
            files->add(val);
        }
        delete currentFiles;
        delete[] value;
    }
    delete result;

    for (auto i = files->getRoot(); i != nullptr; i = *(i->getNext())) {
        auto value = i->getValue();
        printf("%s\n", value);
        delete[] value;
    }
    delete files;
    return 0;
}