#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "retangulos.h"
#include "ver_mundo.h"

void desenhaRetangulos(Retangulos *retangulos, char **mundo);
void imprimeGuia(Retangulos *retangulos);
void imprimeQuadro(Retangulos *retangulos, char **mundo);

////// API
void imprimeMundo(Retangulos *retangulos)
{
    char **mundo = (char **)malloc((retangulos->maxY + 1) * sizeof(char *));
    for (int i = 0; i <= retangulos->maxY; i++)
    {
        mundo[i] = (char *)malloc((retangulos->maxX + 1) * sizeof(char));
        memset(mundo[i], VAZIO, (retangulos->maxX + 1) * sizeof(char));
    }

    desenhaRetangulos(retangulos, mundo);
    imprimeGuia(retangulos);
    imprimeQuadro(retangulos, mundo);
    imprimeGuia(retangulos);

    for (int i = 0; i <= retangulos->maxY; i++)
        free(mundo[i]);
    free(mundo);
}

void imprimeFusoesPossiveis(Retangulos *retangulos)
{
    FusoesPossiveis fusoesPossiveis = {0};
    listaFusoesPossiveis(retangulos, &fusoesPossiveis);
    if (fusoesPossiveis.total)
    {
        printf("💬 Possíveis fusões:\n");
        for (int i = 0; i < fusoesPossiveis.total; i++)
        {
            FusaoPossivel fusaoPossivel = fusoesPossiveis.lista[i];
            printf("   ⦾ %d,%d + %d,%d\n", fusaoPossivel.a->x, fusaoPossivel.a->y, fusaoPossivel.b->x, fusaoPossivel.b->y);
        }
        free(fusoesPossiveis.lista);
    }
}

void imprimeListaRetangulos(Retangulos *retangulos)
{
    if (retangulos->total == 0) {
        printf("💬 Sem retângulos\n");
        return;
    }
    char str[50];
    for (int r = 0; r < retangulos->total; r++)
    {
        retanguloToString(retangulos->lista[r], str);
        printf("   ⦾ %s\n", str);
    }
}

///// private
void desenhaRetangulo(char **mundo, Retangulo retangulo)
{
    for (int yy = retangulo.y; yy < retangulo.y + retangulo.h; yy++)
        for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
            mundo[yy][xx] = isPontoContorno(retangulo, xx, yy) ? PINTADO_CONTORNO : PINTADO_INTERIOR;
}

void desenhaRetangulos(Retangulos *retangulos, char **mundo)
{
    for (int i = 0; i < retangulos->total; i++)
        desenhaRetangulo(mundo, retangulos->lista[i]);
}

void imprimeQuadro(Retangulos *retangulos, char **mundo)
{
    for (int y = retangulos->maxY; y >= 1; y--)
    {
        for (int x = 1; x <= retangulos->maxX; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
}

void imprimeGuia(Retangulos *retangulos)
{
    for (int x = 1; x <= retangulos->maxX; x++)
        printf(x % 10 == 0 ? "%d" : "-", x / 10);
    printf("\n");
}
