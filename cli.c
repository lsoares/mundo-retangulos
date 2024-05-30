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
void correComandoFundir(Retangulos *retangulos);
void correComandoListar(Retangulos *retangulos);
void correComandoLimpar(Retangulos *retangulos);
void correComandoSair(Retangulos *retangulos);
void imprimeFusoesPossiveis(Retangulos *retangulos);

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
    printf(GREEN "╔═════════════════════╗\n");
    printf("║ ⦿ create x,y+l,h    ║\n");
    printf("║ ⦿ moveright x,y+p   ║\n");
    printf("║ ⦿ moveleft x,y+p    ║\n");
    printf("║ ⦿ merge x₁,y₁+x₂,y₂ ║\n");
    printf("║ ⦿ print             ║\n");
    printf("║ ⦿ clear             ║\n");
    printf("║ ⦿ list              ║\n");
    printf("║ ⦿ exit              ║\n");
    printf("╚═════════════════════╝\n▶ " RESET);
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
    else if (strcmp(comando, "merge") == 0)
        correComandoFundir(retangulos);
    else if (strcmp(comando, "clear") == 0)
        correComandoLimpar(retangulos);
    else if (strcmp(comando, "exit") == 0)
        correComandoSair(retangulos);
    else
        printf("❌ comando inválido: %s\n", comando);
}

void correComandoCriar(Retangulos *retangulos)
{
    int args[4];
    scanf(" %d,%d + %d,%d", &args[0], &args[1], &args[2], &args[3]);
    int resultado = criaRetangulo(retangulos, args[0], args[1], args[2], args[3]);
    if (resultado == CRIAR_TAMANHO_INVALIDO)
        printf("❌ retângulo com tamanho inválido\n");
    else if (resultado == CRIAR_FORA_DO_MUNDO)
        printf("❌ retângulo fora do mundo\n");
    else if (resultado == CRIAR_COLISAO)
        printf("❌ retângulo colide com outro\n");
    else
    {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void imprimeFusoesPossiveis(Retangulos *retangulos)
{
    FusoesPossiveis fusoesPossiveis = {0};
    listaFusoesPossiveis(retangulos, &fusoesPossiveis);
    if (fusoesPossiveis.total)
    {
        printf("ℹ️ Possíveis fusões:\n");
        for (int i = 0; i < fusoesPossiveis.total; i++)
        {
            FusaoPossivel fusaoPossivel = fusoesPossiveis.lista[i];
            printf("   ▬ %d,%d + %d,%d\n", fusaoPossivel.a->x, fusaoPossivel.a->y, fusaoPossivel.b->x, fusaoPossivel.b->y);
        }
        free(fusoesPossiveis.lista);
    }
}

void correComandoMover(Retangulos *retangulos, Direcao direcao)
{
    assert(direcao == ESQUERDA || direcao == DIREITA);
    int args[3];
    scanf(" %d,%d + %d", &args[0], &args[1], &args[2]);
    int resultado = moveRetangulo(retangulos, args[0], args[1], direcao * args[2]);
    if (resultado == MOVER_FORA_DO_MUNDO)
        printf("❌ retângulo fora do mundo\n");
    else if (resultado == MOVER_COLISAO)
        printf("❌ retângulo colide com outro\n");
    else if (resultado == MOVER_RET_NAO_ENCONTRADO)
        printf("❌ retângulo não encontrado\n");
    else
    {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void correComandoFundir(Retangulos *retangulos)
{
    int args[4];
    scanf(" %d,%d + %d,%d", &args[0], &args[1], &args[2], &args[3]);

    int resultado = fundeRetangulos(retangulos, args[0], args[1], args[2], args[3]);

    if (resultado == FUNDIR_RET1_NAO_ENCONTRADO)
        printf("❌ retângulo 1 não encontrado\n");
    if (resultado == FUNDIR_RET2_NAO_ENCONTRADO)
        printf("❌ retângulo 2 não encontrado\n");
    if (resultado == FUNDIR_FUSAO_INVALIDA)
        printf("❌ fusão inválida\n");
    else if (resultado == FUNDIR_OK)
    {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}
void correComandoListar(Retangulos *retangulos)
{
    char str[50];
    for (int r = 0; r < retangulos->total; r++)
    {
        retanguloToString(retangulos->lista[r], str);
        printf("   ▬ %s\n", str);
    }
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
