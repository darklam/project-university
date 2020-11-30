#include "TextProcessing.hpp"
#include "Utils.hpp"
#include <thread>
#include <mutex>

Vector2D TextProcessing::tokenizePlus(FastVector<std::string>& texts) {
  auto coreCount = std::thread::hardware_concurrency();
  FastVector<std::string>* threadData[coreCount];
  for (int i = 0; i < coreCount; i++) {
    threadData[i] = new FastVector<std::string>(texts.getLength() / coreCount);
  }

  for (int i = 0; i < texts.getLength(); i++) {
    auto data = threadData[i % coreCount];
    data->append(texts[i]);
    std::cout << texts[i] << std::endl;
  }

  std::mutex m;
  std::thread handles[coreCount];
  Vector2D output = new FastVector<FastVector<std::string>*>(texts.getLength());

  for (int i = 0; i < coreCount; i++) {
    auto currData = threadData[i];

    handles[i] = std::thread([&](FastVector<std::string> data) {
      for (int j = 0; j < data.getLength(); j++) {
        auto str = data[j];
        auto tokens = new FastVector<std::string>(10);
        std::cout << "String: " << str << std::endl;
        Utils::splitNew(str, " ", *tokens);
        m.lock();
        output->append(tokens);
        m.unlock();
      }
    }, *currData);
  }

  for (int i = 0; i < coreCount; i++) {
    handles[i].join();
  }
  return output;
}