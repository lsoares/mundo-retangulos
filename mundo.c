#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERRO_FORA_MUNDO 11
#define ERRO_COLISAO 12
#define RETANGULO_NAO_ENCONTRADO 21
#define PINTADO 'x'
#define VAZIO ' '

typedef struct
{
    int x, y, l, h;
} Retangulo;

typedef struct
{
    Retangulo *lista;
    int total;
} Retangulos;

void desenhaRetangulo(char mundo[26][81], Retangulo retangulo)
{
    for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
    {
        mundo[retangulo.y][xx] = PINTADO;
        mundo[retangulo.y + retangulo.h - 1][xx] = PINTADO;
    }
    for (int yy = retangulo.y + 1; yy < retangulo.y + retangulo.h - 1; yy++)
    {
        mundo[yy][retangulo.x] = PINTADO;
        mundo[yy][retangulo.x + retangulo.l - 1] = PINTADO;
    }
}

void trataImprimirMundo(Retangulos *retangulos)
{
    char mundo[26][81];
    // limpar
    for (int y = 1; y <= 25; y++)
    {
        for (int x = 1; x <= 80; x++)
            mundo[y][x] = VAZIO;
    }
    // desenhar retangulos
    for (int r = 0; r < retangulos->total; r++)
        desenhaRetangulo(mundo, retangulos->lista[r]);
    // imprimir
    for (int y = 25; y >= 1; y--)
    {
        for (int x = 1; x <= 80; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
}

bool verificaDentroMundo(Retangulo retangulo)
{
    return retangulo.x >= 1 && retangulo.y >= 1 && retangulo.x + retangulo.l <= 80 && retangulo.y + retangulo.h <= 25;
}

void printRetanguloCoords(Retangulo retangulo)
{
    printf("Coordenadas do Retângulo: x=%d, y=%d, comprimento=%d, altura=%d\n", retangulo.x, retangulo.y, retangulo.l, retangulo.h);
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

int trataCriarRetangulo(Retangulos *retangulos, int x, int y, int l, int h)
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

int trataMoverRetangulo(Retangulos *retangulos, int x, int y, int p)
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

int main()
{
    char comando[20];
    int args[4];
    Retangulos retangulos = {};

    while (true)
    {
        printf("\n📖 📖 📖 O que fazer? 📖 📖 📖 \n");
        printf(" ⦿ create x,y+l,h\n");
        printf(" ⦿ moveright x,y+p\n");
        printf(" ⦿ moveleft x,y+p\n");
        printf(" ⦿ print\n");
        printf(" ⦿ exit\n");
        printf("\n");

        scanf("%s", comando);

        if (strcmp(comando, "create") == 0)
        {
            scanf("%d,%d+%d,%d", &args[0], &args[1], &args[2], &args[3]);
            int resultado = trataCriarRetangulo(&retangulos, args[0], args[1], args[2], args[3]);
            if (resultado == ERRO_FORA_MUNDO)
                printf("❌ retângulo fora do mundo.\n");
            else if (resultado == ERRO_COLISAO)
                printf("❌ retângulo colide com outro.\n");
            else
                trataImprimirMundo(&retangulos);
        }
        else if (strcmp(comando, "print") == 0)
        {
            trataImprimirMundo(&retangulos);
        }
        else if (strcmp(comando, "moveleft") == 0 || strcmp(comando, "moveright") == 0)
        {
            scanf("%d,%d+%d", &args[0], &args[1], &args[2]);
            int mult = strcmp(comando, "moveleft") == 0 ? -1 : 1;
            int resultado = trataMoverRetangulo(&retangulos, args[0], args[1], mult * args[2]);
            if (resultado == ERRO_FORA_MUNDO)
                printf("❌ retângulo fora do mundo.\n");
            else if (resultado == ERRO_COLISAO)
                printf("❌ retângulo colide com outro.\n");
            else if (resultado == RETANGULO_NAO_ENCONTRADO)
                printf("❌ retângulo não encontrado.\n");
            else
                trataImprimirMundo(&retangulos);
        }
        else if (strcmp(comando, "exit") == 0)
        {
            free(retangulos.lista);
            exit(0);
        }
        else
            printf("❌ comando inválido: %s\n", comando);
    }

    return 0;
}

/*
create 1,3+12,5
create 9,6+11,3
create 18,10+6,3
moveleft 12,7+3
*/