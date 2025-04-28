#pragma once

#include <stdbool.h>

#define MAX_OUTPUT_LEN 15000

int run(const char *executable, char *output);
int runWithInput(const char *input, const char *command, char *output);

bool containsText(const char *result, const char *text);
bool equalInts(int expected, int actual);
