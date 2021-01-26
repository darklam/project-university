#include <iostream>
#include <string>
#include "Sort.hpp"
#include "FastVector.hpp"
#include "HashMap.hpp"

void Sort::sortMax(int length, FastVector<Entry<int>*>& src, FastVector<int>& dest){
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


void Sort::sortMin(int length, FastVector<Entry<int>*>& src, FastVector<int>& dest){
    for(int i = 0; i < length; i++){
        int min = 9999999999999999;
        int pos = -1;
        for(int j = 0; j < src.getLength(); j++){
            if(dest.includes(j)) continue;
            auto entry = src.get(j);
            if(min > entry->value){
                min = entry->value;
                pos = j;
            }
        }
        dest.append(pos);
    }
}


void Sort::sort(int length, FastVector<float>& src, FastVector<int>& dest){
    for(int i = 0; i < length; i++){
        int max = -9999999999999999;
        int pos = -1;
        for(int j = 0; j < src.getLength(); j++){
            if(dest.includes(j)) continue;
            auto entry = src.get(j);
            if(max < entry){
                max = entry;
                pos = j;
            }
        }
        dest.append(pos);
    }
}