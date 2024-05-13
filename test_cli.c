#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <regex.h>
#include <stdbool.h>

void executarCli(char *outputPrograma, ...)
{
    // prepara comandos: converte ["m","3","103","S"] em "m\n3\n103\nS"
    va_list comandosUt;
    va_start(comandosUt, outputPrograma);
    char comandosUtComNewline[300] = "";
    char *arg;
    while ((arg = va_arg(comandosUt, char *)) != NULL)
        sprintf(comandosUtComNewline + strlen(comandosUtComNewline), "%s\n", arg);
    va_end(comandosUt);
    // executa programa: converte "m\n3\n103\nS" em "echo '"m\n3\n103\nS"' | cli ./cli.exe" e corre
    char comandoComExecutavel[320] = "";
    sprintf(comandoComExecutavel, "echo '%s' | ./cli.exe", comandosUtComNewline);
    FILE *pipe = popen(comandoComExecutavel, "r");
    assert(pipe);
    // coleta output do programa (com um pipe de leitura)
    outputPrograma[0] = '\0';
    char path[1000];
    while (fgets(path, sizeof(path), pipe))
        strcat(outputPrograma, path);
    assert(!pclose(pipe));
}

bool contemTexto(const char *resultado, const char *texto)
{
    bool contido = strstr(resultado, texto);
    if (!contido)
    {
        fprintf(stderr, "%s\n", resultado);
        fflush(stderr);
    }
    return contido;
}

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
    char output[5000];

    executarCli(output,
                "create 1,3+12,5",
                "create 9,6+11,3",
                // "create 18,10+6,3",
                // "moveleft 12,7+3",
                "exit", NULL);

    const char *esperado =
        "        xxxxxxxxxxx                                                             \n"
        "        x         x                                                             \n"
        "        xxxxxxxxxxx                                                             \n";
    // "xxxxxxxxxxxx                                                                   \n"
    // "x          x                                                                   \n"
    // "x          x     xxxxxx                                                        \n"
    // "x          x     x    x                                                        \n"
    // "xxxxxxxxxxxx     xxxxxx                                                        \n";
    assert(contemTexto(output, esperado));
}

int main()
{
    test_um_retangulo();
    test_enunciado();
    return 0;
}
