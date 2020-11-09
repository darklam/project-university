#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>
#include "CSV.hpp"
#include "Clique.hpp"
#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "JSON.hpp"
#include "List.hpp"
#include "Set.hpp"
#include "Utils.hpp"

int main() {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  clock_t begin = clock();
  auto path = FileSystem::join(cwd, "W_Dataset.csv");
  auto clique = new Clique();
  auto pairs = CSV::ReadCSV(path);

  HashMap<Set*> map;

  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    if (pair->value == 0) {
      continue;
    }
    HashResult<Set*> res1;
    map.get(pair->id1, &res1);
    HashResult<Set*> res2;
    map.get(pair->id2, &res2);

    if (res1.hasValue && res2.hasValue) {
      if (res1.value != res2.value) {
        res1.value->merge(res2.value);
        auto entries = res2.value->getItems();
        for (auto it = entries->getRoot(); it != nullptr; it = *(it->getNext())) {
          auto v = it->getValue();
          auto actual = v->value;
          HashResult<Set*> res3;
          map.get(actual, &res3);
          if (res3.hasValue) {
            map.set(actual, res1.value);
          }
          delete v;
        }
        delete entries;
        delete res2.value;
        map.set(pair->id2, res1.value);
        continue;
      }
      res1.value->add(pair->id2);
    } else if (res1.hasValue && !res2.hasValue) {
      res1.value->add(pair->id2);
      map.set(pair->id2, res1.value);
      continue;
    } else if (res2.hasValue && !res1.hasValue) {
      res2.value->add(pair->id1);
      continue;
    } else {
      Set* set = new Set();
      set->add(pair->id1);
      set->add(pair->id2);
      map.set(pair->id1, set);
      map.set(pair->id2, set);
      continue;
    }
  }

  auto res = map.getEntries();

  HashMap<Set*> dedupe;
  for (auto j = res->getRoot(); j != nullptr; j = *(j->getNext())) {
    auto val = j->getValue();
    auto item = val->value;
    std::string key = std::to_string((intptr_t) item);
    dedupe.set(key, item);
    delete val;
  }
  delete res;

  auto unique = dedupe.getEntries();
  for (auto i = unique->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    for (auto i = items->getRoot(); i != nullptr; i = *(i->getNext())) {
      auto val = i->getValue();
      std::cout << val->value << ",";
      delete val;
    }
    std::cout << std::endl;
    delete cur;
  }

  delete unique;

  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    delete pair;
  }
  delete pairs;
  return 0;
}