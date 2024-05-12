#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERRO_FORA_MUNDO 11
#define ERRO_COLISAO 12

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
        mundo[retangulo.y][xx] = 'x';
        mundo[retangulo.y + retangulo.h - 1][xx] = 'x';
    }
    for (int yy = retangulo.y + 1; yy < retangulo.y + retangulo.h - 1; yy++)
    {
        mundo[yy][retangulo.x] = 'x';
        mundo[yy][retangulo.x + retangulo.l - 1] = 'x';
    }
}

void trataImprimirMundo(Retangulos *retangulos)
{
    char mundo[26][81];
    // limpar
    for (int y = 1; y <= 25; y++)
    {
        for (int x = 1; x <= 80; x++)
            mundo[y][x] = '`';
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

bool detetaColisao(Retangulo a, Retangulo b)
{
    return a.y + a.h >= b.y && a.y <= b.y + b.h && a.x + a.l >= b.x && a.x <= b.x + b.l;
}

bool detetaColisoes(Retangulos *retangulos, Retangulo retangulo)
{
    for (int r = 0; r < retangulos->total; r++)
    {
        if (detetaColisao(retangulos->lista[r], retangulo))
            return true;
    }
    return false;
}

int trataCriarRetangulo(Retangulos *retangulos, int x, int y, int l, int h)
{
    Retangulo novoRetangulo = (Retangulo){x, y, l, h};
    if (!verificaDentroMundo(novoRetangulo))
        return ERRO_FORA_MUNDO;
    if (detetaColisoes(retangulos, novoRetangulo)) {
        return ERRO_COLISAO;
    }
    retangulos->lista = realloc(retangulos->lista, (retangulos->total + 1) * sizeof(Retangulo));
    retangulos->lista[retangulos->total] = novoRetangulo;
    retangulos->total++;
    trataImprimirMundo(retangulos);
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
            if (resultado == ERRO_COLISAO)
                printf("❌ retângulo colide com outro.\n");
            continue;
        }
        if (strcmp(comando, "print") == 0)
        {
            trataImprimirMundo(&retangulos);
            continue;
        }

        if (strcmp(comando, "exit") == 0)
            exit(0);

        printf("❌ comando inválido: %s\n", comando);
    }

    return 0;
}