#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include "testing.h"

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
    char linha[1000];
    while (fgets(linha, sizeof(linha), pipe))
        strcat(outputPrograma, linha);
    assert(!pclose(pipe));
    fflush(stdout);
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