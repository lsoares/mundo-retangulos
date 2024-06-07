#include <stdbool.h>

#pragma once

#define MAX_OUTPUT_LEN 15000

int runCommand(const char *executable, char *output);
int pipeToRunCommand(const char *input, const char *command, char *output);
bool containsText(const char *resultado, const char *texto);
