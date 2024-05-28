#pragma once

#define LARGURA_MUNDO 80
#define ALTURA_MUNDO 25

typedef struct
{
    int x, y, l, h;
} Retangulo;

typedef struct
{
    Retangulo *lista;
    int total;
} Retangulos;

typedef enum
{
    CRIAR_OK = 0,
    CRIAR_TAMANHO_INVALIDO,
    CRIAR_FORA_DO_MUNDO,
    CRIAR_COLISAO
} ResultadoCriar;
ResultadoCriar criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h);
typedef enum
{
    MOVER_OK = 0,
    MOVER_TAMANHO_INVALIDO,
    MOVER_FORA_DO_MUNDO,
    MOVER_COLISAO,
    MOVER_RET_NAO_ENCONTRADO,
} ResultadoMover;
ResultadoMover moveRetangulo(Retangulos *retangulos, int x, int y, int p);
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

void listaFusoesPossiveis(Retangulos *retangulos, FusoesPossiveis *fusoesPossiveis);

typedef enum
{
    FUNDIR_OK = 0,
    FUNDIR_RET1_NAO_ENCONTRADO,
    FUNDIR_RET2_NAO_ENCONTRADO,
    FUNDIR_FUSAO_INVALIDA
} ResultadoFundir;
ResultadoFundir fundeRetangulos(Retangulos *retangulos, int x1, int y1, int x2, int y2);