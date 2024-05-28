#include <stdio.h>
#include <stdbool.h>
#include "retangulos.h"
#include "ver_mundo.h"

bool isPontoContorno(Retangulo retangulo, int x, int y)
{
    return y == retangulo.y || y == retangulo.y + retangulo.h - 1 ||
           x == retangulo.x || x == retangulo.x + retangulo.l - 1;
}

void desenhaRetangulo(char mundo[][LARGURA_MUNDO + 1], Retangulo retangulo)
{
    for (int yy = retangulo.y; yy < retangulo.y + retangulo.h; yy++)
        for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
            mundo[yy][xx] = isPontoContorno(retangulo, xx, yy) ? PINTADO_CONTORNO : PINTADO_INTERIOR;
}

void imprimeMundo(Retangulos *retangulos)
{
    // um extra em x,y para obrigar a começar em 1,1 e evitar contas
    char mundo[ALTURA_MUNDO + 1][LARGURA_MUNDO + 1];
    // limpa
    for (int y = 1; y <= ALTURA_MUNDO; y++)
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            mundo[y][x] = VAZIO;
    // desenha retangulos
    for (int i = 0; i < retangulos->total; i++)
        desenhaRetangulo(mundo, retangulos->lista[i]);
    // imprime
    for (int y = ALTURA_MUNDO; y >= 1; y--)
    {
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
}