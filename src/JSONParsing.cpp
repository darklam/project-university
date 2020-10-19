#include "JSONParsing.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

void JSONParsing::parseJSON(char* path) {
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Welp something was not okie dokie, errno: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  while (getline(&line, &len, fp) != -1) {
    printf("%s\n", line);
  }

  fclose(fp);
  if (line != NULL) {
    free(line);
  }
}