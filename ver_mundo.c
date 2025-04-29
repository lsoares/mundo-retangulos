#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "retangulos.h"
#include "ver_mundo.h"

char **preparaMundo(const Retangulos *retangulos);
void desenhaRetangulos(const Retangulos *retangulos, char **mundo);
void imprimeGuia(const Retangulos *retangulos);
void imprimeQuadro(const Retangulos *retangulos, char **mundo);
void apagaMundo(const Retangulos *retangulos, char **mundo);

////// API
void imprimeMundo(const Retangulos *retangulos)
{
    char **mundo = preparaMundo(retangulos);
    desenhaRetangulos(retangulos, mundo);
    imprimeGuia(retangulos);
    imprimeQuadro(retangulos, mundo);
    imprimeGuia(retangulos);
    apagaMundo(retangulos, mundo);
}

void apagaMundo(const Retangulos *retangulos, char **mundo)
{
    for (int i = 0; i <= retangulos->maxY; i++)
        free(mundo[i]);
    free(mundo);
}

char **preparaMundo(const Retangulos *retangulos)
{
    char **mundo = (char **)malloc((retangulos->maxY + 1) * sizeof(char *));
    for (int i = 0; i <= retangulos->maxY; i++)
    {
        mundo[i] = (char *)malloc((retangulos->maxX + 1) * sizeof(char));
        memset(mundo[i], VAZIO, (retangulos->maxX + 1) * sizeof(char));
    }
    return mundo;
}

void imprimeFusoesPossiveis(const Retangulos *retangulos)
{
    FusoesPossiveis fusoesPossiveis = {0};
    listaFusoesPossiveis(retangulos, &fusoesPossiveis);
    if (fusoesPossiveis.total)
    {
        printf("💬 Possíveis fusões:\n");
        for (size_t i = 0; i < fusoesPossiveis.total; i++)
        {
            FusaoPossivel fusaoPossivel = fusoesPossiveis.lista[i];
            printf("   ⦾ %d,%d + %d,%d\n", fusaoPossivel.a->x, fusaoPossivel.a->y, fusaoPossivel.b->x, fusaoPossivel.b->y);
        }
    }
    free(fusoesPossiveis.lista);
}

void imprimeListaRetangulos(const Retangulos *retangulos)
{
    if (retangulos->total == 0)
    {
        printf("💬 Sem retângulos\n");
        return;
    }
    char str[50];
    for (size_t r = 0; r < retangulos->total; r++)
    {
        retanguloToString(&retangulos->lista[r], str);
        printf("   ⦾ %s\n", str);
    }
}

///// private
void desenhaRetangulo(char **mundo, const Retangulo* retangulo)
{
    for (int yy = retangulo->y; yy < retangulo->y + retangulo->h; yy++)
        for (int xx = retangulo->x; xx < retangulo->x + retangulo->l; xx++)
            mundo[yy][xx] = isPontoContorno(retangulo, xx, yy) ? PINTADO_CONTORNO : PINTADO_INTERIOR;
}

void desenhaRetangulos(const Retangulos *retangulos, char **mundo)
{
    for (size_t i = 0; i < retangulos->total; i++)
        desenhaRetangulo(mundo, &retangulos->lista[i]);
}

void imprimeQuadro(const Retangulos *retangulos, char **mundo)
{
    for (int y = retangulos->maxY; y >= 1; y--)
    {
        for (int x = 1; x <= retangulos->maxX; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
}

void imprimeGuia(const Retangulos *retangulos)
{
    for (int x = 1; x <= retangulos->maxX; x++)
        printf(x % 10 == 0 ? "%d" : "-", x / 10);
    printf("\n");
}
