#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "retangulos.h"

bool estaDentroMundo(Retangulo retangulo)
{
    bool dentroEmX = retangulo.x >= 1 && retangulo.x + retangulo.l <= LARGURA_MUNDO + 1;
    bool dentroEmY = retangulo.y >= 1 && retangulo.y + retangulo.h <= ALTURA_MUNDO + 1;
    return dentroEmX && dentroEmY;
}

bool detetaColisao(Retangulo a, Retangulo b)
{
    bool entreX = (a.x < (b.x + b.l)) && ((a.x + a.l) > b.x);
    bool entreY = (a.y < (b.y + b.h)) && ((a.y + a.h) > b.y);
    return entreX && entreY;
}

bool detetaColisoes(Retangulos *retangulos, Retangulo *retangulo)
{
    for (int r = 0; r < retangulos->total; r++)
        if (&retangulos->lista[r] != retangulo // ignora ele próprio
            && detetaColisao(retangulos->lista[r], *retangulo))
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
        do
            ret->y--; // avança enquanto for possível
        while (!detetaColisoes(retangulos, ret) && estaDentroMundo(*ret));
        ret->y++; // anula último movimento visto que foi inválido
    }
}

// TODO: return error enum
int criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h)
{
    Retangulo novoRetangulo = (Retangulo){.x = x, .y = y, .l = l, .h = h};
    if (l < 1 || h < 1)
        return TAMANHO_INVALIDO;
    if (!estaDentroMundo(novoRetangulo))
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
    Retangulo ponto = {x, y, 1, 1};
    for (int i = 0; i < retangulos->total; i++)
    {
        Retangulo *ret = &(retangulos->lista[i]);
        if (detetaColisao(ponto, *ret))
            return ret;
    }
    return NULL;
}

// TODO: return error enum
int moveRetangulo(Retangulos *retangulos, int x, int y, int p)
{
    Retangulo *ret = procuraRetangulo(retangulos, x, y);
    if (!ret)
        return RET_NAO_ENCONTRADO;

    int antigoX = ret->x;
    ret->x += p;
    if (!estaDentroMundo(*ret))
    {
        ret->x = antigoX; // colocar onde estava pois sairia do mundo
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

bool isPontoContorno(Retangulo retangulo, int x, int y)
{
    return y == retangulo.y || y == retangulo.y + retangulo.h - 1 ||
           x == retangulo.x || x == retangulo.x + retangulo.l - 1;
}

void imprimeRetangulo(Retangulo retangulo)
{
    printf("   ▬ %2d,%2d + %2d,%2d\n", retangulo.x, retangulo.y, retangulo.l, retangulo.h);
}

void limpaRetangulos(Retangulos *retangulos)
{
    free(retangulos->lista);
    retangulos->total = 0;
    retangulos->lista = NULL;
}

bool verificaFusaoPossivel(Retangulo *a, Retangulo *b)
{
    bool mesmaLagura = a->x == b->x && a->x + a->l == b->x + b->l;
    bool sobrepostosA = a->y + a->h == b->y;
    bool sobrepostosB = b->y + b->h == a->y;
    return mesmaLagura && (sobrepostosA || sobrepostosB);
}

void listaFusoesPossiveis(Retangulos *retangulos, FusoesPossiveis *fusoesPossiveis)
{
    for (int i = 0; i < retangulos->total; i++)
    {
        Retangulo *a = &retangulos->lista[i];
        for (int k = i + 1; k < retangulos->total; k++)
        {
            Retangulo *b = &retangulos->lista[k];
            if (verificaFusaoPossivel(a, b))
            {
                fusoesPossiveis->lista = realloc(fusoesPossiveis->lista, sizeof(FusaoPossivel) * (fusoesPossiveis->total + 1));
                fusoesPossiveis->lista[fusoesPossiveis->total] = (FusaoPossivel){a, b};
                fusoesPossiveis->total++;
            }
        }
    }
}

void apagaRetangulo(Retangulos *retangulos, Retangulo *retangulo)
{
    *retangulo = retangulos->lista[retangulos->total - 1]; // sobrepõe o retangulo a apagar com o último
    retangulos->total--;
    retangulos->lista = realloc(retangulos->lista, retangulos->total * sizeof(Retangulo));
}

int min(int a, int b)
{
    return a < b ? a : b;
}

ErroFusao fundeRetangulos(Retangulos *retangulos, int x1, int y1, int x2, int y2)
{
    Retangulo *ret1 = procuraRetangulo(retangulos, x1, y1);
    if (ret1 == NULL)
        return RET1_NAO_ENCONTRADO;
    Retangulo *ret2 = procuraRetangulo(retangulos, x2, y2);
    if (ret2 == NULL)
        return RET2_NAO_ENCONTRADO;
    if (!verificaFusaoPossivel(ret1, ret2))
        return FUSAO_INVALIDA;
        
    ret2->y = min(ret1->y, ret2->y);
    ret2->h = ret1->h + ret2->h;
    apagaRetangulo(retangulos, ret1);

    return OK;
}
