#include <stdio.h>
#include "retangulos.h"
#include "ver_mundo.h"

// um extra em x,y para obrigar a começar em 1,1 e evitar contas
typedef char Mundo[ALTURA_MUNDO + 1][LARGURA_MUNDO + 1];

void desenhaRetangulo(Mundo mundo, Retangulo retangulo)
{
    // pinta arestas horizontais
    for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
    {
        mundo[retangulo.y][xx] = PINTADO;
        mundo[retangulo.y + retangulo.h - 1][xx] = PINTADO;
    }
    // pinta arestas verticais
    for (int yy = retangulo.y + 1; yy < retangulo.y + retangulo.h - 1; yy++)
    {
        mundo[yy][retangulo.x] = PINTADO;
        mundo[yy][retangulo.x + retangulo.l - 1] = PINTADO;
    }
}

void imprimirMundo(Retangulos *retangulos)
{
    Mundo mundo;
    // limpa
    for (int y = 1; y <= ALTURA_MUNDO; y++)
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            mundo[y][x] = VAZIO;
    // desenha retangulos
    for (int r = 0; r < retangulos->total; r++)
        desenhaRetangulo(mundo, retangulos->lista[r]);
    // imprime
    for (int y = ALTURA_MUNDO; y >= 1; y--)
    {
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
}