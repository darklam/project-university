#include "gtest/gtest.h"
#include "FastVector.hpp"

TEST(FastVector, append){
    FastVector<int> vec;
    for(int i = 0; i < 1000; i++){
        vec.append(i);
    }

    for (int i = 0; i < 1000; i++) {
        auto val = vec[i];
        ASSERT_EQ(val, i);
    }

    ASSERT_EQ(vec.getLength(), 1000);
}

TEST(FastVector, remove){
    FastVector<int> vec;
    for(int i = 0; i < 1000; i++){
        vec.append(i);
    }

    for(int i = 0; i < 500; i++){
        vec.remove(0);
    }

    for (int i = 0; i < 500; i++) {
        auto val = vec[i];
        ASSERT_EQ(val, i + 500);
    }

    ASSERT_EQ(vec.getLength(), 500);
}