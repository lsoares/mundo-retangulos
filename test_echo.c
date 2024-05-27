#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>

bool containsText(const char *text, const char *search)
{
    bool contained = strstr(text, search) != NULL;
    if (!contained)
    {
        fprintf(stderr, "%s\n", text);
        fprintf(stderr, "❗️ The text '%s' not found above.\n", search);
    }
    return contained;
}

int runCliCommand(char *executable, char *output)
{
    FILE *pipe = popen(executable, "r");
    assert(pipe);
    output[0] = '\0';
    char row[1000];
    while (fgets(row, sizeof(row), pipe))
        strcat(output, row);
    int status = pclose(pipe);
    assert(status != -1);
    return WEXITSTATUS(status);
}

void test_ps()
{
    char output[1000];

    runCliCommand("ps", output);

    assert(containsText(output, "PID TTY           TIME CMD"));
}

void executeCmd()
{
    FILE *fp = popen("sort", "r+");
    assert(fp != NULL);
    fprintf(fp, "banana\napple\norange\n");
    fflush(fp);
    char output[10000];
    output[0] = '\0';
    char row[1000];
    while (fgets(row, sizeof(row), fp)) {
        printf("c");
        strcat(output, row);

    }
    assert(pclose(fp) != -1);
}

void test_x() {
    executeCmd();
}

bool equalStrings(const char *expected, const char *actual) {
    if (strcmp(expected, actual) != 0) {
        fprintf(stderr, "❌ Strings are not equal.\nExpected: \"%s\"\nActual:   \"%s\"\n", expected, actual);
        return false;
    }
    return true;
}

// echo "hello world" | tr 'l' 'L'
void test_tr()
{
    char output[1000];

    runCliCommand("echo 'hello world' | tr 'l' 'L'", output);

    assert(equalStrings("heLLo worLd\n", output));
}

int pipeToRunCommand(const char *input, const char *command, char *output) {
    char fullCommand[2048];
    snprintf(fullCommand, sizeof(fullCommand), "echo '%s' | %s", input, command);
    return runCliCommand(fullCommand, output);
}

void test_sort()
{
    char output[1000];

    pipeToRunCommand(
        "banana\n"
        "apple\n"
        "orange",
        "sort --ignore-case", output);

    assert(equalStrings(
        "apple\n"
        "banana\n"
        "orange\n",
        output));
}

void test_fail() {
    char output[100];

    int result = pipeToRunCommand("", "sort -1", output);

    assert(2 == result);

}

typedef struct
{
    int x, y, r;
} Circle;

void printCircle(const Circle circle)
{
    printf("At: (%d, %d), Radius: %d\n", circle.x, circle.y, circle.r);
}

bool circlesAreEqual(const Circle expected, const Circle actual)
{
    if (expected.x != actual.x || expected.y != actual.y || expected.r != actual.r)
    {
        fprintf(stderr, "❌ Circles are not equal.\nExpected: ");
        printCircle(expected);
        fprintf(stderr, "Actual:   ");
        printCircle(actual);
        return false;
    }
    return true;
}

int main()
{
    test_tr();
    test_ps();
    test_sort();
    test_fail();
    // test_circles();

    return 0;
}
