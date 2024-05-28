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

void imprimeMundo(Retangulos *retangulos)
{
    char mundo[ALTURA_MUNDO + 1][LARGURA_MUNDO + 1]; // extra para referencial em 1,1 e evitar contas
    memset(mundo, VAZIO, sizeof(mundo));             // limpa
    for (int i = 0; i < retangulos->total; i++)      // desenha retangulos
        desenhaRetangulo(mundo, retangulos->lista[i]);
    for (int x = 1; x <= LARGURA_MUNDO; x++)
        printf(x < LARGURA_MUNDO ? "-" : "\n");
    for (int y = ALTURA_MUNDO; y >= 1; y--) // imprime
    {
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
    for (int x = 1; x <= LARGURA_MUNDO; x++)
        printf(x < LARGURA_MUNDO ? "-" : "-\n");
}