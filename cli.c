#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulos.h"
#include "ver_mundo.h"

void imprimeMenu();

void correComando(Retangulos *retangulos, const char *comando);

void correComandoCriar(Retangulos *retangulos);

void correComandoMover(const Retangulos *retangulos, int multiplicador);

void correComandoApagar(Retangulos *retangulos);

void correComandoFundir(Retangulos *retangulos);

void correComandoLimpar(Retangulos *retangulos);

void correComandoSair(Retangulos *retangulos);

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char) str[i]);
}

int main() {
    system("chcp 65001 > nul 2>&1 || true");
    printf("▭ ▭ ▭ ▭ Bem-vindo/a ao Mundo dos Retângulos ▭ ▭ ▭ ▭\n");
    Retangulos retangulos = {.maxX = 80, .maxY = 25};
    imprimeMenu();
    while (true) {
        printf("\033[92m" "┈➤ " "\033[0m"); // green
        char comando[20];
        scanf(" %19s", comando);
        toLowerCase(comando);
        correComando(&retangulos, comando);
    }
}

void imprimeMenu() {
    printf("\033[92m"); // green
    printf("╔═══════════════════╦═══════╗\n");
    printf("║ create x,y+l,h    ║ clear ║\n");
    printf("║ moveright x,y+p   ║ list  ║\n");
    printf("║ moveleft x,y+p    ║ print ║\n");
    printf("║ merge x₁,y₁+x₂,y₂ ║       ║\n");
    printf("║ delete x,y        ║ exit  ║\n");
    printf("╚═══════════════════╩═══════╝\n");
    printf("\033[0m"); // reset color
    fflush(stdout);
}

void correComando(Retangulos *retangulos, const char *comando) {
    if (strcmp(comando, "create") == 0)
        correComandoCriar(retangulos);
    else if (strcmp(comando, "moveleft") == 0)
        correComandoMover(retangulos, -1);
    else if (strcmp(comando, "moveright") == 0)
        correComandoMover(retangulos, 1);
    else if (strcmp(comando, "merge") == 0)
        correComandoFundir(retangulos);
    else if (strcmp(comando, "delete") == 0)
        correComandoApagar(retangulos);
    else if (strcmp(comando, "print") == 0)
        imprimeMundo(retangulos);
    else if (strcmp(comando, "list") == 0)
        imprimeListaRetangulos(retangulos);
    else if (strcmp(comando, "clear") == 0)
        correComandoLimpar(retangulos);
    else if (strcmp(comando, "exit") == 0)
        correComandoSair(retangulos);
    else {
        printf("⚠️ comando inválido\n");
        imprimeMenu();
    }
}

void correComandoCriar(Retangulos *retangulos) {
    int args[4];
    scanf(" %d,%d + %d,%d", &args[0], &args[1], &args[2], &args[3]);
    const int resultado = criaRetangulo(retangulos, args[0], args[1], args[2], args[3]);
    if (resultado == CRIAR_TAMANHO_INVALIDO)
        printf("⚠️ retângulo com tamanho inválido\n");
    else if (resultado == CRIAR_FORA_DO_MUNDO)
        printf("⚠️ retângulo fora do mundo\n");
    else if (resultado == CRIAR_COLISAO)
        printf("⚠️ retângulo colide com outro\n");
    else {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void correComandoMover(const Retangulos *retangulos, const int multiplicador) {
    int args[3];
    scanf(" %d,%d + %d", &args[0], &args[1], &args[2]);
    const int resultado = moveRetangulo(retangulos, args[0], args[1], multiplicador * args[2]);
    if (resultado == MOVER_FORA_DO_MUNDO)
        printf("⚠️ retângulo fora do mundo\n");
    else if (resultado == MOVER_COLISAO)
        printf("⚠️ retângulo colide com outro\n");
    else if (resultado == MOVER_INEXISTENTE)
        printf("⚠️ retângulo não encontrado\n");
    else {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void correComandoFundir(Retangulos *retangulos) {
    int args[4];
    scanf(" %d,%d + %d,%d", &args[0], &args[1], &args[2], &args[3]);
    const int resultado = fundeRetangulos(retangulos, args[0], args[1], args[2], args[3]);
    if (resultado == FUNDIR_RET1_INEXISTENTE)
        printf("⚠️ retângulo 1 não encontrado\n");
    if (resultado == FUNDIR_RET2_INEXISTENTE)
        printf("⚠️ retângulo 2 não encontrado\n");
    if (resultado == FUNDIR_FUSAO_INVALIDA)
        printf("⚠️ fusão inválida\n");
    else if (resultado == FUNDIR_OK) {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void correComandoApagar(Retangulos *retangulos) {
    int args[2];
    scanf(" %d,%d", &args[0], &args[1]);
    const int resultado = apagaRetangulo(retangulos, args[0], args[1]);
    if (resultado == APAGAR_INEXISTENTE)
        printf("⚠️ retângulo não encontrado\n");
}

void correComandoLimpar(Retangulos *retangulos) {
    limpaRetangulos(retangulos);
    imprimeMundo(retangulos);
}

void correComandoSair(Retangulos *retangulos) {
    printf("💬 Até à próxima\n");
    limpaRetangulos(retangulos);
    exit(0);
}
