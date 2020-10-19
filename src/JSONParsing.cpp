#include "JSONParsing.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

void JSONParsing::parseJSON(const char* path, const char* filename) {
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Welp something was not okie dokie, errno: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  bool finished = false;

  while (!finished) {
    int length = getline(&line, &len, fp);
    if (length == -1) {
      finished = true;
      continue;
    }
    printf("%s\n", line);
  }

  fclose(fp);
  if (line != NULL) {
    free(line);
  }
}