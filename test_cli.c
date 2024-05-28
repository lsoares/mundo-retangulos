#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "testing.h"

void test_um_retangulo()
{
    char output[5000];

    int erro = pipeToRunCommand(
        "create 2,3+12,5\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(erro == 0);
    const char *esperado =
        " XXXXXXXXXXXX                                                                   \n"
        " XOOOOOOOOOOX                                                                   \n"
        " XOOOOOOOOOOX                                                                   \n"
        " XOOOOOOOOOOX                                                                   \n"
        " XXXXXXXXXXXX                                                                   \n";
    assert(containsText(output, esperado));
}

void test_gravidade()
{
    char output[20000];

    pipeToRunCommand(
        "create 38,20+4,4\n"
        "create 38,20+4,4\n"
        "create 38,20+4,4\n"
        "create 38,20+4,4\n"
        "moveleft 38,1+10\n"
        "list\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "▬ 28, 1 +  4, 4"));
    assert(containsText(output, "▬ 38, 1 +  4, 4"));
    assert(containsText(output, "▬ 38, 5 +  4, 4"));
    assert(containsText(output, "▬ 38, 9 +  4, 4"));
}

void test_gravidade_apoiado_no_extremo()
{
    char output[20000];

    pipeToRunCommand("create 1,1+1,1\n"
                     "create 1,2+80,1\n"
                     "exit\n",
                     "./cli.exe",
                     output);

    char *esperado =
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
        "X                                                                               \n";
    assert(containsText(output, esperado));
}

void test_enunciado()
{
    char output[20000];

    pipeToRunCommand(
        "create 1,3+12,5\n"
        "create 9,6+11,3\n"
        "create 18,10+6,3\n"
        "moveleft 12,7+3\n"
        "exit\n",
        "./cli.exe",
        output);

    char *esperado =
        "     XXXXXXXXXXX                                                                \n"
        "     XOOOOOOOOOX                                                                \n"
        "     XXXXXXXXXXX                                                                \n"
        "XXXXXXXXXXXX                                                                    \n"
        "XOOOOOOOOOOX                                                                    \n"
        "XOOOOOOOOOOX     XXXXXX                                                         \n"
        "XOOOOOOOOOOX     XOOOOX                                                         \n"
        "XXXXXXXXXXXX     XXXXXX                                                         \n";
    assert(containsText(output, esperado));
}

void test_desenhar_fora_do_mundo()
{
    char output[1000];

    pipeToRunCommand(
        "create 120,50+10,5\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "retângulo fora do mundo"));
}

void test_colisao_desenhar()
{
    char output[5000];

    pipeToRunCommand(
        "create 5,5+10,10\n"
        "create 10,5+10,10\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "retângulo colide com outro"));
}

void test_colisao_mover()
{
    char output[10000];

    pipeToRunCommand(
        "create 5,5+10,10\n"
        "create 20,5+10,10\n"
        "moveleft 20,1+10"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "retângulo colide com outro"));
}

void test_colisao_mover_para_fora_do_mundo()
{
    char output[5000];

    pipeToRunCommand(
        "create 5,5+10,10\n"
        "moveleft 20,1+80\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "retângulo não encontrado"));
}

void test_retangulo_invalido()
{
    char output[5000];

    pipeToRunCommand(
        "create 5,5+10,0\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "retângulo com tamanho inválido"));
}

void test_menu()
{
    char output[5000];

    pipeToRunCommand("exit\n", "./cli.exe", output);

    assert(containsText(output, " create x,y+l,h "));
    assert(containsText(output, " moveright x,y+p "));
    assert(containsText(output, " moveleft x,y+p "));
    assert(containsText(output, " merge x₁,y₁+x₂,y₂ "));
    assert(containsText(output, " print "));
    assert(containsText(output, " clear "));
    assert(containsText(output, " list "));
    assert(containsText(output, " exit "));
}

void test_comando_invalido()
{
    char output[5000];

    pipeToRunCommand(
        "create 5,5+10,10\n"
        "xyz\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "comando inválido"));
}

void test_fusao_mostrar_info()
{
    char output[10000];

    pipeToRunCommand(
        "create 5,1+4,2\n"
        "create 5,15+4,3\n"
        "exit\n",
        "./cli.exe",
        output);

    assert(containsText(output, "Possíveis fusões"));
    assert(containsText(output, "5,1 + 5,3"));
}

void test_fundir_retangulos()
{
    char output[10000];

    pipeToRunCommand(
        "create 5,1+4,2\n"
        "create 5,15+4,3\n"
        "merge 5,1 + 5,3\n"
        "exit\n",
        "./cli.exe",
        output);

    char *esperado =
        "    XXXX                                                                        \n"
        "    XOOX                                                                        \n"
        "    XOOX                                                                        \n"
        "    XOOX                                                                        \n"
        "    XXXX                                                                        \n";
    assert(containsText(output, esperado));
}

int main()
{
    // criar
    test_um_retangulo();
    test_retangulo_invalido();
    test_desenhar_fora_do_mundo();
    test_colisao_desenhar();

    // mover
    test_colisao_mover();
    test_colisao_mover_para_fora_do_mundo();

    // fundir
    test_fusao_mostrar_info();
    test_fundir_retangulos();
    // TODO: testar fusao invalida
    // TODO: testar retangulo nao existe

    // gravidade
    test_enunciado();
    test_gravidade();
    test_gravidade_apoiado_no_extremo();

    // geral
    test_menu();
    test_comando_invalido();

    return 0;
}
