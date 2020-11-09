#include <unistd.h>
#include <iostream>
#include <string>
#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "JSON.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include "CSV.hpp"
#include "Clique.hpp"
#include <ctime>

int main() {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  clock_t begin = clock();
  auto path = FileSystem::join(cwd, "W_Dataset.csv");
  auto clique = new Clique();
  auto pairs = CSV::ReadCSV(path);
  std::cout << "Finished Reading" << std::endl;
  // for (auto i = 0; i < pairs->getLength(); i++) { 
  //   auto pair =(*pairs)[i];
  //   delete pair;
  // }
  // delete pairs;
  // printf("got pairs\n");

  // clock_t time1 = clock();
  for (auto i = 0; i < pairs->getLength(); i++) { 
    auto pair =(*pairs)[i];
    // if(pair->getMatch() == 0) continue;
    // clique->Add(pair->getId1());
    // clique->Add(pair->getId2());
    // clique->Pair(pair->getId1(), pair->getId2());
  }
  // // std::string item1("AA");
  // // clique->Add(item1);
  // // std::string item2("1");
  // // clique->Add(item2);
  // // std::string item3("2");
  // // clique->Add(item3);
  // // std::string item4("3");
  // // clique->Add(item4);
  // // std::string item5("4");
  // // clique->Add(item5);
  // // clique->Pair(item1, item2);
  // // clique->Pair(item3, item4);
  // // clique->Pair(item1, item5);
  // clock_t time2 = clock();
  // printf("get entries\n");
  //auto entries = clique->getEntries();
  // // for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) { 
  // //   auto entry = i->getValue();
  // //   //printf("%s\n", entry->key);
  // //   std::cout << entry->key << std::endl;
  // //   auto list = entry->value;
  // //   for (auto j = list->getRoot(); j != nullptr; j = *(j->getNext())) { 
  // //     std::cout << j->getValue() << " ";
  // //   }
  // //   printf("\n");
  // //   //delete list;
  // //   delete entry;
  // // }
  // // delete entries;
  // clock_t time3 = clock();
  // printf("write file\n");
  // std::string file("W_Output.csv");
  // CSV::WriteCSV(file, entries);
  for (auto i = 0; i < pairs->getLength(); i++) { 
    auto pair =(*pairs)[i];
    delete pair;
  }
  delete pairs;
  // clock_t end = clock();
  // delete clique;
  clock_t end = clock();
  std::cout << double(end - begin) / CLOCKS_PER_SEC << std::endl;
  // std::cout << double(time2 - time1) / CLOCKS_PER_SEC << std::endl;
  // std::cout << double(time3 - time2) / CLOCKS_PER_SEC << std::endl;
  // std::cout << double(end - time3) / CLOCKS_PER_SEC << std::endl;
  return 0;
}