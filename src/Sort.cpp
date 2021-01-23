#include <iostream>
#include <string>
#include "Sort.hpp"
#include "FastVector.hpp"
#include "HashMap.hpp"

void Sort::sort(int length, FastVector<Entry<int>*>& src, FastVector<int>& dest){
    for(int i = 0; i < length; i++){
        int max = -9999999999999999;
        int pos = -1;
        for(int j = 0; j < src.getLength(); j++){
            if(dest.includes(j)) continue;
            auto entry = src.get(j);
            if(max < entry->value){
                max = entry->value;
                pos = j;
            }
        }
        dest.append(pos);
    }
}