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

void test_desenhar_fora_do_mundo()
{
    char output[1000];

    executarCli(output,
                "create 120,50+10,5",
                "exit",
                NULL);

    assert(contemTexto(output, "❌ retângulo fora do mundo"));
}

void test_colisao_desenhar()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "create 10,5+10,10",
                "exit",
                NULL);

    assert(contemTexto(output, "❌ retângulo colide com outro"));
}

void test_colisao_mover()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "create 20,5+10,10",
                "moveleft 20,1+10"
                "exit",
                NULL);

    assert(contemTexto(output, "❌ retângulo colide com outro"));
}

void test_colisao_mover_para_fora_do_mundo()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "moveleft 20,1+80"
                "exit",
                NULL);

    assert(contemTexto(output, "❌ retângulo não encontrado"));
}

void test_comando_invalido()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "xyz",
                "exit",
                NULL);

    assert(contemTexto(output, "❌ comando inválido"));
}

int main()
{
    test_um_retangulo();
    test_enunciado();
    test_desenhar_fora_do_mundo();
    test_colisao_desenhar();
    test_colisao_mover();
    test_colisao_mover_para_fora_do_mundo();
    test_comando_invalido();

    return 0;
}
