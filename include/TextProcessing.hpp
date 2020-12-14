#ifndef TEXT_PROCESSING_H
#define TEXT_PROCESSING_H
#include "FastVector.hpp"
#include "Types.hpp"

namespace TextProcessing {
  Vector2D tokenizePlus(FastVector<std::string>& texts);
  void getStopWords(FastVector<std::string>& words);
}

#endif