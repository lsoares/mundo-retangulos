#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "testing.h"

int runCommand(const char *executable, char *output)
{
    char command[1024];
    snprintf(command, sizeof(command), "%s 2>&1", executable); // redirect stderr to stdout
    FILE *pipe = popen(command, "r");
    assert(pipe);
    output[0] = '\0';
    char row[1000];
    while (fgets(row, sizeof(row), pipe))
    {
        assert (strlen(output) + strlen(row) < MAX_OUTPUT_LEN);
        strcat(output, row);
    }
    int exitStatus = pclose(pipe);
    assert(exitStatus != -1);
    return WEXITSTATUS(exitStatus); // return the executable exit status
}

int pipeToRunCommand(const char *input, const char *command, char *output)
{
    char fullCommand[2048];
    snprintf(fullCommand, sizeof(fullCommand), "echo '%s' | %s", input, command);
    return runCommand(fullCommand, output);
}

bool containsText(const char *text, const char *search)
{
    if (strstr(text, search) == NULL)
    {
        fprintf(stderr, "%s\n", text);
        fprintf(stderr, "❌ The text '%s' was not found above.\n", search);
        return false;
    }
    return true;
}