#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "retangulos.h"

bool verificaDentroMundo(Retangulo retangulo)
{
    // TODO: considerar se largura/altura do mundo fazem parte do conceito de guardar retangulos
    return retangulo.x >= 1 && retangulo.y >= 1 && retangulo.x + retangulo.l <= LARGURA_MUNDO && retangulo.y + retangulo.h <= ALTURA_MUNDO;
}

bool detetaColisao(Retangulo a, Retangulo b)
{
    bool colisaoX = (a.x < (b.x + b.l)) && ((a.x + a.l) > b.x);
    bool colisaoY = (a.y < (b.y + b.h)) && ((a.y + a.h) > b.y);
    return colisaoX && colisaoY;
}

bool detetaColisoes(Retangulos *retangulos, Retangulo *retangulo)
{
    for (int r = 0; r < retangulos->total; r++)
    {
        if (&retangulos->lista[r] == retangulo) // ignora ele próprio
            continue;
        if (detetaColisao(retangulos->lista[r], *retangulo))
            return true;
    }
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
        do
        {
            ret->y--; // avança até haver colisão
        } while (!detetaColisoes(retangulos, ret) && verificaDentroMundo(*ret));
        ret->y++; // anula último movimento visto que foi de colisão
    }
}

int criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h)
{
    Retangulo novoRetangulo = (Retangulo){x, y, l, h};
    if (l < 1 || h < 1)
        return TAMANHO_INVALIDO;
    if (!verificaDentroMundo(novoRetangulo))
        return FORA_DO_MUNDO;
    if (detetaColisoes(retangulos, &novoRetangulo))
        return COLISAO;

    retangulos->lista = realloc(retangulos->lista, (retangulos->total + 1) * sizeof(Retangulo));
    retangulos->lista[retangulos->total] = novoRetangulo;
    retangulos->total++;

    aplicaGravidade(retangulos);
    return 0;
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
        return RET_NAO_ENCONTRADO;

    int antigoX = ret->x;
    ret->x += p;
    if (!verificaDentroMundo(*ret))
    {
        ret->x = antigoX;
        return FORA_DO_MUNDO;
    }
    if (detetaColisoes(retangulos, ret))
    {
        ret->x = antigoX; // colocar onde estava pois houve colisão
        return COLISAO;
    }
    aplicaGravidade(retangulos);
    return 0;
}

void imprimeRetangulo(Retangulo retangulo)
{
    printf("   ▬ %2d,%2d + %2d,%2d\n", retangulo.x, retangulo.y, retangulo.l, retangulo.h);
}

void limpaRetangulos(Retangulos *retangulos)
{
    retangulos->total = 0;
    free(retangulos->lista);
}