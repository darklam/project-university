#include "acutest.h"

void example(void) {
  TEST_ASSERT(1 == 1);
}

TEST_LIST = {
  { "example", example },
  { NULL, NULL }
};