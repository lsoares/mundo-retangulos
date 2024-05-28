#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "retangulos.h"
#include "ver_mundo.h"

typedef enum
{
    ESQUERDA = -1,
    DIREITA = 1
} Direcao;

void imprimeMenu();
void correComando(Retangulos *retangulos, char *comando);
void correComandoCriar(Retangulos *retangulos);
void correComandoMover(Retangulos *retangulos, Direcao direcao);
void correComandoListar(Retangulos *retangulos);
void correComandoLimpar(Retangulos *retangulos);
void correComandoSair(Retangulos *retangulos);
void imprimeMergesPossiveis(Retangulos *retangulos);

int main()
{
    Retangulos retangulos = {};
    while (true)
    {
        imprimeMenu();
        char comando[20];
        scanf(" %19s", comando);
        correComando(&retangulos, comando);
    }
    return 0;
}

#define GREEN "\033[32m"
#define RESET "\033[0m"
void imprimeMenu()
{
    printf(GREEN "╔═══════════════════╗\n");
    printf("║ ⦿ create x,y+l,h  ║\n");
    printf("║ ⦿ moveright x,y+p ║\n");
    printf("║ ⦿ moveleft x,y+p  ║\n");
    printf("║ ⦿ print           ║\n");
    printf("║ ⦿ clear           ║\n");
    printf("║ ⦿ list            ║\n");
    printf("║ ⦿ exit            ║\n");
    printf("╚═══════════════════╝\n▶ " RESET);
}

void correComando(Retangulos *retangulos, char *comando)
{
    if (strcmp(comando, "create") == 0)
        correComandoCriar(retangulos);
    else if (strcmp(comando, "print") == 0)
        imprimeMundo(retangulos);
    else if (strcmp(comando, "list") == 0)
        correComandoListar(retangulos);
    else if (strcmp(comando, "moveleft") == 0)
        correComandoMover(retangulos, ESQUERDA);
    else if (strcmp(comando, "moveright") == 0)
        correComandoMover(retangulos, DIREITA);
    // TODO: comando merge. e.g. merge 23,2+24,6
    else if (strcmp(comando, "clear") == 0)
        correComandoLimpar(retangulos);
    else if (strcmp(comando, "exit") == 0)
        correComandoSair(retangulos);
    else
        printf("❌ comando inválido\n");
}

void correComandoCriar(Retangulos *retangulos)
{
    int args[4];
    scanf("%d,%d+%d,%d", &args[0], &args[1], &args[2], &args[3]);
    int resultado = criaRetangulo(retangulos, args[0], args[1], args[2], args[3]);
    if (resultado == TAMANHO_INVALIDO)
        printf("❌ retângulo com tamanho inválido\n");
    else if (resultado == FORA_DO_MUNDO)
        printf("❌ retângulo fora do mundo\n");
    else if (resultado == COLISAO)
        printf("❌ retângulo colide com outro\n");
    else
    {
        imprimeMundo(retangulos);
        imprimeMergesPossiveis(retangulos);
    }
}

void imprimeMergesPossiveis(Retangulos *retangulos)
{
    MergesPossiveis mergesPossiveis = {0};
    detetaMergesPossiveis(retangulos, &mergesPossiveis);
    if (mergesPossiveis.total)
    {
        printf("ℹ️ Possíveis merges:\n");
        for (int i = 0; i < mergesPossiveis.total; i++)
        {
            MergePossivel mergePossivel = mergesPossiveis.lista[i];
            printf("   ▬ %d,%d + %d,%d\n", mergePossivel.a->x, mergePossivel.a->y, mergePossivel.b->x, mergePossivel.b->y);
        }
        free(mergesPossiveis.lista);
    }
}

void correComandoMover(Retangulos *retangulos, Direcao direcao)
{
    assert(direcao == ESQUERDA || direcao == DIREITA);
    int args[3];
    scanf("%d,%d+%d", &args[0], &args[1], &args[2]);
    int resultado = moveRetangulo(retangulos, args[0], args[1], direcao * args[2]);
    if (resultado == FORA_DO_MUNDO)
        printf("❌ retângulo fora do mundo\n");
    else if (resultado == COLISAO)
        printf("❌ retângulo colide com outro\n");
    else if (resultado == RET_NAO_ENCONTRADO)
        printf("❌ retângulo não encontrado\n");
    else
    {
        imprimeMundo(retangulos);
        imprimeMergesPossiveis(retangulos);
    }
}

void correComandoListar(Retangulos *retangulos)
{
    for (int r = 0; r < retangulos->total; r++)
        imprimeRetangulo(retangulos->lista[r]);
}

void correComandoLimpar(Retangulos *retangulos)
{
    limpaRetangulos(retangulos);
    imprimeMundo(retangulos);
}

void correComandoSair(Retangulos *retangulos)
{
    limpaRetangulos(retangulos);
    exit(0);
}
