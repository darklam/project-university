#include <iostream>
#include "JSONParsing.hpp"
#include "FileSystem.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include "HashMap.hpp"
#include <unistd.h>

int main() {
    int len = 2048;
    char cwd[len];
    getcwd(cwd, len);
    auto path = FileSystem::join(cwd, "datasets/cameras/buy.net/4233.json");
    printf("%s\n", path);
    auto result = JSONParsing::parseJSON(path, "whatever");
    // auto path = "../datasets/cameras";
    auto props = result->getProperties()->getEntries();

    for (auto i = props->getRoot(); i != nullptr; i = *(i->getNext())) {
        auto cur = i->getValue();
        auto val = cur->value;
        if (val->isArray) {
            continue;
        }
        printf("Key: %s, Value: %s\n", cur->key, val->value);
        delete cur;
        delete val;
    }
    delete props;
    delete result;
    delete[] path;
    // auto files = FileSystem::getAllFiles(path);

    // for (auto i = files->getRoot(); i != nullptr; i = *(i->getNext())) {
    //     auto value = i->getValue();
    //     printf("%s\n", value);
    //     delete[] value;
    // }
    // delete files;
    return 0;
}