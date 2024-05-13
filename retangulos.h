#define LARGURA_MUNDO 80
#define ALTURA_MUNDO 25

#define ERRO_FORA_MUNDO 11
#define ERRO_COLISAO 12
#define RETANGULO_NAO_ENCONTRADO 21

typedef struct
{
    int x, y, l, h;
} Retangulo;

typedef struct
{
    Retangulo *lista;
    int total;
} Retangulos;

int moveRetangulo(Retangulos *retangulos, int x, int y, int p);
int criaRetangulo(Retangulos *retangulos, int x, int y, int l, int h);
