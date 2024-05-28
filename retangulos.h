#pragma once

#define LARGURA_MUNDO 80
#define ALTURA_MUNDO 25

#define FORA_DO_MUNDO 11
#define COLISAO 12
#define TAMANHO_INVALIDO 13
#define RET_NAO_ENCONTRADO 21

typedef struct
{
    int x, y, l, h;
} Retangulo;

typedef struct
{
    Retangulo *lista;
    int total;
} Retangulos;

int criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h);
int moveRetangulo(Retangulos *retangulos, int x, int y, int p);
bool isPontoContorno(Retangulo retangulo, int x, int y);
void imprimeRetangulo(Retangulo retangulo);
void limpaRetangulos(Retangulos *retangulos);

typedef struct
{
    Retangulo *a;
    Retangulo *b;
} FusaoPossivel;

typedef struct
{
    FusaoPossivel *lista;
    int total;
} FusoesPossiveis;

void listaFusoesPossiveis(Retangulos *retangulos, FusoesPossiveis *mergesPossiveis);

typedef enum
{
    OK = 0,
    RET1_NAO_ENCONTRADO,
    RET2_NAO_ENCONTRADO,
    FUSAO_INVALIDA
} ErroFusao;
ErroFusao fundeRetangulos(Retangulos *retangulos, int x1, int y1, int x2, int y2);