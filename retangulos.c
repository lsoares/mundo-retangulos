#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "retangulos.h"

bool estaDentroLimites(const Retangulos *retangulos, const Retangulo *retangulo);

bool estaEmZonaVazia(const Retangulos *retangulos, const Retangulo *retangulo);

void aplicaGravidade(const Retangulos *retangulos);

void aplicaGravidadeRet(const Retangulos *retangulos, Retangulo *ret);

Retangulo *procuraRetangulo(const Retangulos *retangulos, int x, int y);

bool verificaFusaoPossivel(const Retangulo *a, const Retangulo *b);

void apaga(Retangulos *retangulos, Retangulo *retangulo);

////// API
ResultadoCriar criaRetangulo(Retangulos *retangulos, const int x, const int y, const int l, const int h) {
    if (l < 1 || h < 1) return CRIAR_TAMANHO_INVALIDO;
    const Retangulo novoRetangulo = (Retangulo){.x = x, .y = y, .l = l, .h = h};
    if (!estaDentroLimites(retangulos, &novoRetangulo)) return CRIAR_FORA_DO_MUNDO;
    if (!estaEmZonaVazia(retangulos, &novoRetangulo)) return CRIAR_COLISAO;

    retangulos->lista = realloc(retangulos->lista, (retangulos->total + 1) * sizeof(Retangulo));
    assert(retangulos->lista);
    retangulos->lista[retangulos->total] = novoRetangulo;
    retangulos->total++;

    aplicaGravidadeRet(retangulos, &retangulos->lista[retangulos->total] - 1);
    return CRIAR_OK;
}

ResultadoMover moveRetangulo(const Retangulos *retangulos, const int x, const int y, const int p) {
    Retangulo *ret = procuraRetangulo(retangulos, x, y);
    if (!ret) return MOVER_INEXISTENTE;

    const int xOriginal = ret->x;
    const int passo = p > 0 ? 1 : -1;
    for (int i = 0; i < abs(p); i++) {
        ret->x += passo;
        if (!estaDentroLimites(retangulos, ret)) {
            ret->x = xOriginal; // desfaz todo o movimento
            return MOVER_FORA_DO_MUNDO;
        }
        if (!estaEmZonaVazia(retangulos, ret)) {
            ret->x = xOriginal; // desfaz todo o movimento
            return MOVER_COLISAO;
        }
    }
    aplicaGravidade(retangulos);
    return MOVER_OK;
}

void listaFusoesPossiveis(const Retangulos *retangulos, FusoesPossiveis *fusoesPossiveis) {
    const size_t maiorNrFusoesPossivel = retangulos->total * (retangulos->total - 1) / 2;
    fusoesPossiveis->lista = malloc(sizeof(FusaoPossivel) * maiorNrFusoesPossivel);
    assert(fusoesPossiveis->lista);
    fusoesPossiveis->total = 0;

    for (size_t i = 0; i < retangulos->total; i++) {
        Retangulo *a = &retangulos->lista[i];
        for (size_t k = i + 1; k < retangulos->total; k++) {
            Retangulo *b = &retangulos->lista[k];
            if (verificaFusaoPossivel(a, b)) {
                fusoesPossiveis->lista[fusoesPossiveis->total] = (FusaoPossivel){.a = a, .b = b};
                fusoesPossiveis->total++;
            }
        }
    }
}

ResultadoFundir fundeRetangulos(Retangulos *retangulos, const int x1, const int y1, const int x2, const int y2) {
    Retangulo *ret1 = procuraRetangulo(retangulos, x1, y1);
    if (!ret1) return FUNDIR_RET1_INEXISTENTE;
    Retangulo *ret2 = procuraRetangulo(retangulos, x2, y2);
    if (!ret2) return FUNDIR_RET2_INEXISTENTE;
    if (!verificaFusaoPossivel(ret1, ret2)) return FUNDIR_FUSAO_INVALIDA;

    ret2->y = ret1->y < ret2->y ? ret1->y : ret2->y; // novo y = min y
    ret2->h = ret1->h + ret2->h; // novo h = somar os h
    apaga(retangulos, ret1);

    return FUNDIR_OK;
}

ResultadoApagar apagaRetangulo(Retangulos *retangulos, const int x, const int y) {
    Retangulo *ret = procuraRetangulo(retangulos, x, y);
    if (!ret) return APAGAR_INEXISTENTE;

    apaga(retangulos, ret);
    return APAGAR_OK;
}

////// private
bool estaDentroLimites(const Retangulos *retangulos, const Retangulo *retangulo) {
    // começamos a desenhar em 1,1 e não 0,0 para evitar conversões
    const bool dentroEmX = retangulo->x - 1 >= 0 &&
                           retangulo->x + retangulo->l - 1 <= retangulos->maxX;
    const bool dentroEmY = retangulo->y - 1 >= 0 &&
                           retangulo->y + retangulo->h - 1 <= retangulos->maxY;
    return dentroEmX && dentroEmY;
}

bool colidem(const Retangulo *a, const Retangulo *b) {
    const bool entreX = a->x < b->x + b->l &&
                        a->x + a->l > b->x;
    const bool entreY = a->y < b->y + b->h &&
                        a->y + a->h > b->y;
    return entreX && entreY;
}

bool estaEmZonaVazia(const Retangulos *retangulos, const Retangulo *retangulo) {
    for (size_t r = 0; r < retangulos->total; r++)
        if (&retangulos->lista[r] != retangulo // ignora ele próprio
            && colidem(&retangulos->lista[r], retangulo))
            return false;
    return true;
}

static int comparaPorY(const void *a, const void *b) {
    return ((Retangulo *) a)->y - ((Retangulo *) b)->y;
}

void ordenaRetangulosPorY(const Retangulos *retangulos) {
    qsort(retangulos->lista, retangulos->total, sizeof(Retangulo), comparaPorY);
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
        aplicaGravidadeRet(retangulos, &retangulos->lista[i]);
}

Retangulo *procuraRetangulo(const Retangulos *retangulos, const int x, const int y) {
    const Retangulo ponto = {.x = x, .y = y, .l = 1, .h = 1};
    for (size_t i = 0; i < retangulos->total; i++) {
        Retangulo *ret = &retangulos->lista[i];
        if (colidem(&ponto, ret))
            return ret;
    }
    return NULL;
}

bool pertenceAoContorno(const Retangulo *retangulo, const int x, const int y) {
    return y == retangulo->y || y == retangulo->y + retangulo->h - 1 ||
           x == retangulo->x || x == retangulo->x + retangulo->l - 1;
}

void retanguloToString(const Retangulo *retangulo, char *str) {
    if (!retangulo)
        sprintf(str, "NULL");
    else
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
           (a->y + a->h == b->y || // A em cima de B
            b->y + b->h == a->y); // ou B em cima de A
}

void apaga(Retangulos *retangulos, Retangulo *retangulo) {
    *retangulo = retangulos->lista[retangulos->total - 1]; // sobrepõe o retangulo a apagar com o último
    retangulos->total--;
    if (retangulos->total == 0) {
        free(retangulos->lista);
        retangulos->lista = NULL;
        return;
    }

    retangulos->lista = realloc(retangulos->lista, retangulos->total * sizeof(Retangulo));
    assert(retangulos->lista);

    aplicaGravidade(retangulos);
}
