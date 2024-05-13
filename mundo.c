#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulos.h"

#define PINTADO 'x'
#define VAZIO ' '

void desenhaRetangulo(char mundo[LARGURA_MUNDO + 1][ALTURA_MUNDO + 1], Retangulo retangulo)
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
    char mundo[LARGURA_MUNDO + 1][ALTURA_MUNDO + 1];
    // limpar
    for (int y = 1; y <= LARGURA_MUNDO; y++)
    {
        for (int x = 1; x <= ALTURA_MUNDO; x++)
            mundo[y][x] = VAZIO;
    }
    // desenhar retangulos
    for (int r = 0; r < retangulos->total; r++)
        desenhaRetangulo(mundo, retangulos->lista[r]);
    // imprimir
    for (int y = LARGURA_MUNDO; y >= 1; y--)
    {
        for (int x = 1; x <= ALTURA_MUNDO; x++)
            printf("%c", mundo[y][x]);
        printf("\n");
    }
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
            int resultado = criaRetangulo(&retangulos, args[0], args[1], args[2], args[3]);
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
            int resultado = moveRetangulo(&retangulos, args[0], args[1], mult * args[2]);
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