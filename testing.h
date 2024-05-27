#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>

int runCommand(const char *executable, char *output);
int pipeToRunCommand(const char *input, const char *command, char *output);
bool containsText(const char *resultado, const char *texto);
