#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "testing.h"

void test_um_retangulo()
{
    char output[5000];

    executarCli(output,
                "create 2,3+12,5",
                "exit", NULL);

    const char *esperado =
        " xxxxxxxxxxxx                                                                   \n"
        " x          x                                                                   \n"
        " x          x                                                                   \n"
        " x          x                                                                   \n"
        " xxxxxxxxxxxx                                                                   \n";
    assert(contemTexto(output, esperado));
}

void test_enunciado()
{
    char output[20000];

    executarCli(output,
                "create 1,3+12,5",
                "create 9,6+11,3",
                "create 18,10+6,3",
                "moveleft 12,7+3",
                "exit", NULL);

    char *esperado =
        "     xxxxxxxxxxx                                                                \n"
        "     x         x                                                                \n"
        "     xxxxxxxxxxx                                                                \n"
        "xxxxxxxxxxxx                                                                    \n"
        "x          x                                                                    \n"
        "x          x     xxxxxx                                                         \n"
        "x          x     x    x                                                         \n"
        "xxxxxxxxxxxx     xxxxxx                                                         \n";
    assert(contemTexto(output, esperado));
}

int main()
{
    test_um_retangulo();
    test_enunciado();
    return 0;
}
