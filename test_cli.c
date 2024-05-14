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

void test_gravidade()
{
    char output[20000];

    executarCli(output,
                "create 38,20+4,4",
                "create 38,20+4,4",
                "create 38,20+4,4",
                "create 38,20+4,4",
                "moveleft 38,1+10",
                "list",
                "exit", NULL);

    assert(contemTexto(output, "▬ 28, 1 +  4, 4"));
    assert(contemTexto(output, "▬ 38, 1 +  4, 4"));
    assert(contemTexto(output, "▬ 38, 5 +  4, 4"));
    assert(contemTexto(output, "▬ 38, 9 +  4, 4"));
}

void test_gravidade_apoiado_no_extremo()
{
    char output[20000];

    executarCli(output,
                "create 1,1+1,1",
                "create 1,2+79,2",
                "exit", NULL);

    char *esperado =
        "  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
        "  x                                                                           x\n"
        "  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
        "xxx                                                                            \n"
        "x x                                                                            \n"
        "xxx                                                                            \n";
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

    assert(contemTexto(output, "retângulo fora do mundo"));
}

void test_colisao_desenhar()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "create 10,5+10,10",
                "exit",
                NULL);

    assert(contemTexto(output, "retângulo colide com outro"));
}

void test_colisao_mover()
{
    char output[10000];

    executarCli(output,
                "create 5,5+10,10",
                "create 20,5+10,10",
                "moveleft 20,1+10"
                "exit",
                NULL);

    assert(contemTexto(output, "retângulo colide com outro"));
}

void test_colisao_mover_para_fora_do_mundo()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "moveleft 20,1+80"
                "exit",
                NULL);

    assert(contemTexto(output, "retângulo não encontrado"));
}

void test_retangulo_invalido()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,0",
                "exit",
                NULL);

    assert(contemTexto(output, "retângulo com tamanho inválido"));
}

void test_comando_invalido()
{
    char output[5000];

    executarCli(output,
                "create 5,5+10,10",
                "xyz",
                "exit",
                NULL);

    assert(contemTexto(output, "comando inválido"));
}

int main()
{
    test_um_retangulo();
    test_enunciado();
    test_gravidade();
    // test_gravidade_apoiado_no_extremo();
    test_desenhar_fora_do_mundo();
    test_colisao_desenhar();
    test_colisao_mover();
    test_colisao_mover_para_fora_do_mundo();
    test_comando_invalido();
    test_retangulo_invalido();
    // TODO: testar limpar

    return 0;
}
