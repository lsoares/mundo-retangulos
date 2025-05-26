#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "testing.h"

int run(const char *executable, char *output) {
    FILE *pipe = popen(executable, "r");
    assert(pipe);
    output[0] = '\0';
    char row[1000];
    while (fgets(row, sizeof(row), pipe)) {
        assert(strlen(row) < sizeof(row)); // fails on larger rows. need to increase their size above
        assert(strlen(output) + strlen(row) < MAX_OUTPUT_LEN); // fails on insufficient output size
        strcat(output, row);
    }
    int exitStatus = pclose(pipe);
    assert(exitStatus != -1);
    return WEXITSTATUS(exitStatus); // return the executable exit status
}

int runWithInput(const char *input, const char *command, char *output) {
    char fullCommand[2020];
    assert(strlen(command) + strlen(input) < 2000); // fails if we need a bigger command length
    snprintf(fullCommand, sizeof(fullCommand), "echo '%s' | %s 2>&1", input, command); // redirect stderr to stdout
    return run(fullCommand, output);
}

bool containsText(const char *text, const char *search) {
    if (strstr(text, search) == NULL) {
        fprintf(stderr, "%s\n", text);
        fprintf(stderr, "❌ The text '%s' was not found above.\n", search);
        return false;
    }
    return true;
}

bool equalInts(const int expected, const int actual) {
    if (expected != actual) {
        fprintf(stderr, "❌ Different integers\n");
        fprintf(stderr, "expected: %d\n", expected);
        fprintf(stderr, "actual:   %d\n", actual);
    }
    return expected == actual;
}
