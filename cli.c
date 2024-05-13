#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulos.h"
#include "ver_mundo.h"

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
        printf(" ⦿ list\n");
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
                printf("❌ retângulo fora do mundo\n");
            else if (resultado == ERRO_COLISAO)
                printf("❌ retângulo colide com outro\n");
            else
                imprimirMundo(&retangulos);
        }
        else if (strcmp(comando, "print") == 0)
        {
            imprimirMundo(&retangulos);
        }
        else if (strcmp(comando, "list") == 0)
        {
            for (int r = 0; r < retangulos.total; r++)
                imprimeRetangulo(retangulos.lista[r]);
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
                imprimirMundo(&retangulos);
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
