#ifndef BOW_H
#define BOW_H
#include <string>
#include "FastVector.hpp"
#include "Set.hpp"
#include "Types.hpp"

class BOW {
 public:
  BOW(bool useTfidf);
  BOW();
  void fit(FastVector<std::string> texts);
  Vector2D transform(FastVector<std::string> texts);

 private:
  bool tfidf = false;
  Set s;
};

#endif