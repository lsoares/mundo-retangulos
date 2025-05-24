#pragma once

#include <stdbool.h>

typedef struct {
    int x, y, l, h;
} Retangulo;

void retanguloToString(const Retangulo *retangulo, char *str);

typedef struct {
    Retangulo *lista;
    size_t total;
    int maxX, maxY;
} Retangulos;

typedef enum { CRIAR_OK, CRIAR_TAMANHO_INVALIDO, CRIAR_FORA_DO_MUNDO, CRIAR_COLISAO } ResultadoCriar;

ResultadoCriar criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h);

typedef enum { MOVER_OK, MOVER_FORA_DO_MUNDO, MOVER_COLISAO, MOVER_INEXISTENTE, } ResultadoMover;

ResultadoMover moveRetangulo(const Retangulos *retangulos, int x, int y, int p);

bool isPontoContorno(const Retangulo *retangulo, int x, int y);

typedef enum { APAGAR_OK = 1, APAGAR_INEXISTENTE, } ResultadoApagar;

ResultadoApagar apagaRetangulo(Retangulos *retangulos, int x, int y);

void limpaRetangulos(Retangulos *retangulos);

typedef struct {
    Retangulo *a;
    Retangulo *b;
} FusaoPossivel;

typedef struct {
    FusaoPossivel *lista;
    size_t total;
} FusoesPossiveis;

void listaFusoesPossiveis(const Retangulos *retangulos, FusoesPossiveis *fusoesPossiveis);

typedef enum { FUNDIR_OK, FUNDIR_RET1_INEXISTENTE, FUNDIR_RET2_INEXISTENTE, FUNDIR_FUSAO_INVALIDA } ResultadoFundir;

ResultadoFundir fundeRetangulos(Retangulos *retangulos, int x1, int y1, int x2, int y2);
