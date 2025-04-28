#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "retangulos.h"

bool estaDentroLimites(const Retangulos *retangulos, const Retangulo *retangulo);

bool estaEmZonaVazia(const Retangulos *retangulos, const Retangulo *retangulo);

void aplicaGravidade(const Retangulos *retangulos);

Retangulo *procuraRetangulo(const Retangulos *retangulos, int x, int y);

bool verificaFusaoPossivel(const Retangulo *a, const Retangulo *b);

void apagaRetangulo(Retangulos *retangulos, Retangulo *retangulo);

////// API
ResultadoCriar criaRetangulo(Retangulos *retangulos, const int x, const int y, const int l, const int h) {
    Retangulo novoRetangulo = (Retangulo){.x = x, .y = y, .l = l, .h = h};
    if (l < 1 || h < 1)
        return CRIAR_TAMANHO_INVALIDO;
    if (!estaDentroLimites(retangulos, &novoRetangulo))
        return CRIAR_FORA_DO_MUNDO;
    if (!estaEmZonaVazia(retangulos, &novoRetangulo))
        return CRIAR_COLISAO;

    retangulos->lista = realloc(retangulos->lista, (retangulos->total + 1) * sizeof(Retangulo));
    retangulos->lista[retangulos->total] = novoRetangulo;
    retangulos->total++;

    aplicaGravidade(retangulos);
    return CRIAR_OK;
}

ResultadoMover moveRetangulo(const Retangulos *retangulos, const int x, const int y, const int p) {
    Retangulo *ret = procuraRetangulo(retangulos, x, y);
    if (!ret)
        return MOVER_RET_NAO_ENCONTRADO;

    ret->x += p;
    // temos de mover o retângulo em causa, pois seria mais difícil do que ignorá-lo ao criar um temporário
    if (!estaDentroLimites(retangulos, ret)) {
        ret->x -= p; // anular devido a sair do mundo
        return MOVER_FORA_DO_MUNDO;
    }
    if (!estaEmZonaVazia(retangulos, ret)) {
        ret->x -= p; // anular devido a colisão
        return MOVER_COLISAO;
    }
    aplicaGravidade(retangulos);
    return MOVER_OK;
}

void listaFusoesPossiveis(const Retangulos *retangulos, FusoesPossiveis *fusoesPossiveis) {
    for (size_t i = 0; i < retangulos->total; i++) {
        Retangulo *a = &retangulos->lista[i];
        for (size_t k = i + 1; k < retangulos->total; k++) {
            Retangulo *b = &retangulos->lista[k];
            if (verificaFusaoPossivel(a, b)) {
                fusoesPossiveis->total++;
                fusoesPossiveis->lista = realloc(fusoesPossiveis->lista,
                                                 sizeof(FusaoPossivel) * fusoesPossiveis->total);
                fusoesPossiveis->lista[fusoesPossiveis->total - 1] = (FusaoPossivel){a, b};
            }
        }
    }
}

ResultadoFundir fundeRetangulos(Retangulos *retangulos, const int x1, const int y1, const int x2, const int y2) {
    Retangulo *ret1 = procuraRetangulo(retangulos, x1, y1);
    if (ret1 == NULL)
        return FUNDIR_RET1_NAO_ENCONTRADO;
    Retangulo *ret2 = procuraRetangulo(retangulos, x2, y2);
    if (ret2 == NULL)
        return FUNDIR_RET2_NAO_ENCONTRADO;
    if (!verificaFusaoPossivel(ret1, ret2))
        return FUNDIR_FUSAO_INVALIDA;

    ret2->y = ret1->y < ret2->y ? ret1->y : ret2->y; // novo y = min y
    ret2->h = ret1->h + ret2->h; // novo h = somar os h
    apagaRetangulo(retangulos, ret1);

    return FUNDIR_OK;
}

////// private
bool estaDentroLimites(const Retangulos *retangulos, const Retangulo *retangulo) {
    // começamos a desenhar em 1,1 e não 0,0 para evitar conversões
    bool dentroEmX = (retangulo->x - 1 >= 0) && (retangulo->x + retangulo->l - 1) <= retangulos->maxX;
    bool dentroEmY = (retangulo->y - 1 >= 0) && (retangulo->y + retangulo->h - 1) <= retangulos->maxY;
    return dentroEmX && dentroEmY;
}

bool colidem(const Retangulo *a, const Retangulo *b) {
    bool entreX = (a->x < (b->x + b->l)) && ((a->x + a->l) > b->x);
    bool entreY = (a->y < (b->y + b->h)) && ((a->y + a->h) > b->y);
    return entreX && entreY;
}

bool estaEmZonaVazia(const Retangulos *retangulos, const Retangulo *retangulo) {
    for (size_t r = 0; r < retangulos->total; r++)
        if (&retangulos->lista[r] != retangulo // ignora ele próprio
            && colidem(&retangulos->lista[r], retangulo))
            return false;
    return true;
}

void ordenaRetangulosPorY(const Retangulos *retangulos) {
    for (size_t i = 0; i < retangulos->total - 1; i++)
        for (size_t j = 0; j < retangulos->total - i - 1; j++)
            if (retangulos->lista[j].y > retangulos->lista[j + 1].y) {
                Retangulo temp = retangulos->lista[j];
                retangulos->lista[j] = retangulos->lista[j + 1];
                retangulos->lista[j + 1] = temp;
            }
}

void aplicaGravidadeRet(const Retangulos *retangulos, Retangulo *ret) {
    do
        ret->y--; // avança enquanto for possível
    while (estaDentroLimites(retangulos, ret) && estaEmZonaVazia(retangulos, ret));
    ret->y++; // anula último movimento visto que foi inválido
}

void aplicaGravidade(const Retangulos *retangulos) {
    ordenaRetangulosPorY(retangulos);
    for (size_t i = 0; i < retangulos->total; i++)
        aplicaGravidadeRet(retangulos, &(retangulos->lista[i]));
}

Retangulo *procuraRetangulo(const Retangulos *retangulos, int x, int y) {
    Retangulo ponto = {x, y, 1, 1};
    for (size_t i = 0; i < retangulos->total; i++) {
        Retangulo *ret = &retangulos->lista[i];
        if (colidem(&ponto, ret))
            return ret;
    }
    return NULL;
}

bool isPontoContorno(const Retangulo *retangulo, const int x, const int y) {
    return y == retangulo->y || y == retangulo->y + retangulo->h - 1 ||
           x == retangulo->x || x == retangulo->x + retangulo->l - 1;
}

void retanguloToString(const Retangulo *retangulo, char *str) {
    sprintf(str, "%2d,%2d + %2d,%2d", retangulo->x, retangulo->y, retangulo->l, retangulo->h);
}

void limpaRetangulos(Retangulos *retangulos) {
    free(retangulos->lista);
    retangulos->total = 0;
    retangulos->lista = NULL;
}

bool verificaFusaoPossivel(const Retangulo *a, const Retangulo *b) {
    return a->x == b->x && // mesmo X
           a->l == b->l && // e mesma largura
           (a->y + a->h == b->y // A em cima de B
            || b->y + b->h == a->y); // ou B em cima de A
}

void apagaRetangulo(Retangulos *retangulos, Retangulo *retangulo) {
    *retangulo = retangulos->lista[retangulos->total - 1]; // sobrepõe o retangulo a apagar com o último
    retangulos->total--;
    retangulos->lista = realloc(retangulos->lista, retangulos->total * sizeof(Retangulo));
}
