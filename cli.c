#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulos.h"
#include "ver_mundo.h"

void imprimeMenu();

void trataComando(Retangulos *retangulos, const char *comando);

void trataCriar(Retangulos *retangulos);

void trataMover(const Retangulos *retangulos, int multiplicador);

void trataApagar(Retangulos *retangulos);

void trataFundir(Retangulos *retangulos);

void trataLimpar(Retangulos *retangulos);

void trataSair(Retangulos *retangulos);

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) str[i] = tolower((unsigned char) str[i]);
}

int main() {
    system("chcp 65001 > nul 2>&1 || true");
    printf("▭ ▭ ▭ ▭ Bem-vindo/a ao Mundo dos Retângulos ▭ ▭ ▭ ▭\n");
    Retangulos retangulos = {.maxX = 80, .maxY = 25};
    imprimeMenu();
    char comando[20];
    while (true) {
        printf("\033[92m" "┈➤ " "\033[0m"); // green
        scanf(" %19s", comando);
        toLowerCase(comando);
        trataComando(&retangulos, comando);
        fflush(stdout);
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
}

void trataComando(Retangulos *retangulos, const char *comando) {
    if (strcmp(comando, "create") == 0)
        trataCriar(retangulos);
    else if (strcmp(comando, "moveleft") == 0)
        trataMover(retangulos, -1);
    else if (strcmp(comando, "moveright") == 0)
        trataMover(retangulos, 1);
    else if (strcmp(comando, "merge") == 0)
        trataFundir(retangulos);
    else if (strcmp(comando, "delete") == 0)
        trataApagar(retangulos);
    else if (strcmp(comando, "print") == 0)
        imprimeMundo(retangulos);
    else if (strcmp(comando, "list") == 0)
        imprimeListaRetangulos(retangulos);
    else if (strcmp(comando, "clear") == 0)
        trataLimpar(retangulos);
    else if (strcmp(comando, "exit") == 0)
        trataSair(retangulos);
    else {
        printf("⚠️ comando inválido\n");
        imprimeMenu();
    }
}

void trataCriar(Retangulos *retangulos) {
    int x, y, l, h;
    scanf(" %d , %d + %d , %d", &x, &y, &l, &h);
    const int resultado = criaRetangulo(retangulos, x, y, l, h);
    if (resultado == CRIAR_TAMANHO_INVALIDO)
        printf("⚠️ retângulo com tamanho inválido\n");
    else if (resultado == CRIAR_FORA_DO_MUNDO)
        printf("⚠️ retângulo fora do mundo\n");
    else if (resultado == CRIAR_COLISAO)
        printf("⚠️ retângulo colide com outro\n");
    else if (resultado == CRIAR_OK) {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void trataMover(const Retangulos *retangulos, const int multiplicador) {
    int x, y, p;
    scanf(" %d , %d + %d ", &x, &y, &p);
    const int resultado = moveRetangulo(retangulos, x, y, multiplicador * p);
    if (resultado == MOVER_FORA_DO_MUNDO)
        printf("⚠️ retângulo fora do mundo\n");
    else if (resultado == MOVER_COLISAO)
        printf("⚠️ retângulo colide com outro\n");
    else if (resultado == MOVER_INEXISTENTE)
        printf("⚠️ retângulo não encontrado\n");
    else if (resultado == MOVER_OK) {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void trataFundir(Retangulos *retangulos) {
    int x1, y1, x2, y2;
    scanf(" %d , %d + %d , %d", &x1, &y1, &x2, &y2);
    const int resultado = fundeRetangulos(retangulos, x1, y1, x2, y2);
    if (resultado == FUNDIR_RET1_INEXISTENTE)
        printf("⚠️ retângulo 1 não encontrado\n");
    else if (resultado == FUNDIR_RET2_INEXISTENTE)
        printf("⚠️ retângulo 2 não encontrado\n");
    else if (resultado == FUNDIR_FUSAO_INVALIDA)
        printf("⚠️ fusão inválida\n");
    else if (resultado == FUNDIR_OK) {
        imprimeMundo(retangulos);
        imprimeFusoesPossiveis(retangulos);
    }
}

void trataApagar(Retangulos *retangulos) {
    int x, y;
    scanf(" %d , %d", &x, &y);
    const int resultado = apagaRetangulo(retangulos, x, y);
    if (resultado == APAGAR_INEXISTENTE)
        printf("⚠️ retângulo não encontrado\n");
}

void trataLimpar(Retangulos *retangulos) {
    limpaRetangulos(retangulos);
    imprimeMundo(retangulos);
}

void trataSair(Retangulos *retangulos) {
    printf("💬 Até à próxima\n");
    limpaRetangulos(retangulos);
    exit(0);
}
