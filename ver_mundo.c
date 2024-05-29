#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "retangulos.h"
#include "ver_mundo.h"

void desenhaRetangulo(char mundo[][LARGURA_MUNDO + 1], Retangulo retangulo)
{
    for (int yy = retangulo.y; yy < retangulo.y + retangulo.h; yy++)
        for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
            mundo[yy][xx] = isPontoContorno(retangulo, xx, yy) ? PINTADO_CONTORNO : PINTADO_INTERIOR;
}
void desenhaRetangulos(Retangulos *retangulos, char mundo[][81])
{
    for (int i = 0; i < retangulos->total; i++)
        desenhaRetangulo(mundo, retangulos->lista[i]);
}

void imprimeQuadro(char mundo[][81])
{
    for (int y = ALTURA_MUNDO; y >= 1; y--) // imprime
    {
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
}

void imprimeGuia()
{
    for (int x = 1; x <= LARGURA_MUNDO; x++)
        printf(x % 10 == 0 ? "%d" : "-", x / 10);
    printf("\n");
}

void imprimeMundo(Retangulos *retangulos)
{
    char mundo[ALTURA_MUNDO + 1][LARGURA_MUNDO + 1]; // extra para referencial em 1,1 e evitar contas
    memset(mundo, VAZIO, sizeof(mundo));
    desenhaRetangulos(retangulos, mundo);
    imprimeGuia();
    imprimeQuadro(mundo);
    imprimeGuia();
}
