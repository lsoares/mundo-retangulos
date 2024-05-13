#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "retangulos.h"

bool verificaDentroMundo(Retangulo retangulo)
{
    return retangulo.x >= 1 && retangulo.y >= 1 && retangulo.x + retangulo.l <= 80 && retangulo.y + retangulo.h <= 25;
}

bool detetaColisao(Retangulo a, Retangulo b)
{
    bool colisaoX = (a.x < (b.x + b.l)) && ((a.x + a.l) > b.x);
    bool colisaoY = (a.y < (b.y + b.h)) && ((a.y + a.h) > b.y);
    return colisaoX && colisaoY;
}

bool detetaColisoes(Retangulos *retangulos, Retangulo retangulo)
{
    for (int r = 0; r < retangulos->total; r++)
        if (detetaColisao(retangulos->lista[r], retangulo))
            return true;
    return false;
}

void ordenaRetangulosPorY(Retangulos *retangulos)
{
    for (int i = 0; i < retangulos->total - 1; i++)
        for (int j = 0; j < retangulos->total - i - 1; j++)
            if (retangulos->lista[j].y > retangulos->lista[j + 1].y)
            {
                Retangulo temp = retangulos->lista[j];
                retangulos->lista[j] = retangulos->lista[j + 1];
                retangulos->lista[j + 1] = temp;
            }
}

void aplicaGravidade(Retangulos *retangulos)
{
    ordenaRetangulosPorY(retangulos);
    for (int i = 0; i < retangulos->total; i++)
    {
        Retangulo *ret = &(retangulos->lista[i]);
        Retangulo retanguloPossivel = *ret;
        retanguloPossivel.y++;
        ret->y = 25 + 1; // esconder temporariamente para que nao seja detetado
        do
        {
            retanguloPossivel.y--;
        } while (!detetaColisoes(retangulos, retanguloPossivel) && verificaDentroMundo(retanguloPossivel));
        ret->y = retanguloPossivel.y + 1;
    }
}

int criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h)
{
    Retangulo novoRetangulo = (Retangulo){x, y, l, h};
    if (!verificaDentroMundo(novoRetangulo))
        return ERRO_FORA_MUNDO;
    if (detetaColisoes(retangulos, novoRetangulo))
        return ERRO_COLISAO;

    retangulos->lista = realloc(retangulos->lista, (retangulos->total + 1) * sizeof(Retangulo));
    retangulos->lista[retangulos->total] = novoRetangulo;
    retangulos->total++;

    aplicaGravidade(retangulos);
}

Retangulo *procuraRetangulo(Retangulos *retangulos, int x, int y)
{
    for (int i = 0; i < retangulos->total; i++)
    {
        Retangulo *ret = &(retangulos->lista[i]);
        if (x >= ret->x && x <= ret->x + ret->l && y >= ret->y && y <= ret->y + ret->h)
            return ret;
    }
    return NULL;
}

int moveRetangulo(Retangulos *retangulos, int x, int y, int p)
{
    Retangulo *ret = procuraRetangulo(retangulos, x, y);
    if (!ret)
        return RETANGULO_NAO_ENCONTRADO;

    Retangulo retanguloPossivel = *ret;
    retanguloPossivel.x += p;

    if (!verificaDentroMundo(retanguloPossivel))
        return ERRO_FORA_MUNDO;
    int xBackup = ret->x;
    ret->x = 80 + 1; // esconder temporariamente para que nao seja detetado
    if (detetaColisoes(retangulos, retanguloPossivel))
    {
        ret->x = xBackup; // colocar onde estava
        return ERRO_COLISAO;
    }
    ret->x = retanguloPossivel.x; // move porque não houve colisões
    aplicaGravidade(retangulos);
    return 0;
}
