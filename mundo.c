#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulos.h"

#define PINTADO 'x'
#define VAZIO '-'
// um extra em x,y para obrigar a começar em 1,1
// um extra em x para facilitar impressão
typedef char Mundo[ALTURA_MUNDO + 1][LARGURA_MUNDO + 2];

void desenhaRetangulo(Mundo mundo, Retangulo retangulo)
{
    // pinta arestas horizontais
    for (int xx = retangulo.x; xx < retangulo.x + retangulo.l; xx++)
    {
        mundo[retangulo.y][xx] = PINTADO;
        mundo[retangulo.y + retangulo.h - 1][xx] = PINTADO;
    }
    // pinta arestas verticais
    for (int yy = retangulo.y + 1; yy < retangulo.y + retangulo.h - 1; yy++)
    {
        mundo[yy][retangulo.x] = PINTADO;
        mundo[yy][retangulo.x + retangulo.l - 1] = PINTADO;
    }
}

void trataImprimirMundo(Retangulos *retangulos)
{
    Mundo mundo;
    // limpa
    for (int y = 1; y <= ALTURA_MUNDO; y++)
    {
        mundo[y][LARGURA_MUNDO + 1] = '\0'; // para imprimir cada linha diretamente
        for (int x = 1; x <= LARGURA_MUNDO; x++)
            mundo[y][x] = VAZIO;
    }
    // desenha retangulos
    for (int r = 0; r < retangulos->total; r++)
        desenhaRetangulo(mundo, retangulos->lista[r]);
    // imprime
    for (int y = ALTURA_MUNDO; y >= 1; y--)
    {
        // printf("%s\n", mundo[y]);
       for (int x = 1; x <= LARGURA_MUNDO; x++)
           printf("%c", mundo[y][x]);
       printf("\n");
    }
}

int main()
{
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

        char comando[20];
        scanf("%s", comando);
        if (strcmp(comando, "create") == 0)
        {
            int args[4];
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
            int args[3];
            scanf("%d,%d+%d", &args[0], &args[1], &args[2]);
            int mult = strcmp(comando, "moveleft") == 0 ? -1 : 1;
            int resultado = moveRetangulo(&retangulos, args[0], args[1], mult * args[2]);
            if (resultado == ERRO_FORA_MUNDO)
                printf("❌ retângulo fora do mundo\n");
            else if (resultado == ERRO_COLISAO)
                printf("❌ retângulo colide com outro\n");
            else if (resultado == ERRO_RET_NAO_ENCONTRADO)
                printf("❌ retângulo não encontrado\n");
            else
                trataImprimirMundo(&retangulos);
        }
        else if (strcmp(comando, "exit") == 0)
        {
            limpaRetangulos(&retangulos);
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