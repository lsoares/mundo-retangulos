#include <stdio.h>
#include "retangulos.h"
#include "ver_mundo.h"

void desenhaRetangulo(char mundo[][LARGURA_MUNDO + 1], Retangulo retangulo)
{
    // pinta arestas horizontais
    for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
    {
        mundo[retangulo.y][xx] = PINTADO_CONTORNO;
        mundo[retangulo.y + retangulo.h - 1][xx] = PINTADO_CONTORNO;
    }
    // pinta arestas verticais
    for (int yy = retangulo.y + 1; yy < retangulo.y + retangulo.h - 1; yy++)
    {
        mundo[yy][retangulo.x] = PINTADO_CONTORNO;
        mundo[yy][retangulo.x + retangulo.l - 1] = PINTADO_CONTORNO;
    }
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