#ifndef SORT_HPP
#define SORT_HPP
#include "FastVector.hpp"
#include "HashMap.hpp"

namespace Sort {
    void sortMax(int length, FastVector<Entry<int>*>& src, FastVector<int>& dest);
    void sortMin(int length, FastVector<Entry<int>*>& src, FastVector<int>& dest);
    void sort(int length, FastVector<float>& src, FastVector<int>& dest);
}


#endif

