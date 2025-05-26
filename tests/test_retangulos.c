#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "testing.h"
#include "../retangulos.h"

bool retangulosIguais(const Retangulo esp, const Retangulo atual) {
    char str[50];
    if (esp.x != atual.x || esp.y != atual.y || esp.l != atual.l || esp.h != atual.h) {
        fprintf(stderr, "❌ Retângulos diferem\n");
        retanguloToString(&esp, str);
        fprintf(stderr, "esperado: %s\n", str);
        retanguloToString(&atual, str);
        fprintf(stderr, "atual:    %s\n", str);
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////
// CRIAR
void test_cria_retangulo() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 2, 3);

    assert(equalInts(CRIAR_OK, resultado));
    assert(equalInts(1, retangulos.total));
    assert(retangulosIguais((Retangulo) {1, 1, 2, 3},retangulos.lista[0]));
}

void test_maximo_retangulo() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 80, 25);

    assert(equalInts(CRIAR_OK, resultado));
    assert(equalInts(1, retangulos.total));
    assert(retangulosIguais((Retangulo) {1, 1, 80, 25}, retangulos.lista[0]));
}

void test_minimo_retangulo() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 80, 25, 1, 1); // deixa cair do topo à direita

    assert(equalInts(CRIAR_OK, resultado));
    assert(equalInts(1, retangulos.total));
    assert(retangulosIguais((Retangulo) {80, 1, 1, 1},retangulos.lista[0]));
}

void test_criar_cai_na_base() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    criaRetangulo(&retangulos, 1, 10, 2, 1);

    assert(retangulosIguais((Retangulo) {1, 1, 2, 3},retangulos.lista[0]));
    assert(equalInts(4, retangulos.lista[1].y)); // caiu de 10 para 4
}

void test_criar_cai_noutro() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 5, 2, 3);

    assert(equalInts(CRIAR_OK, resultado));
    assert(retangulosIguais((Retangulo) {1, 1, 2, 3},retangulos.lista[0]));
}

// CRIAR. ERROS
void test_criar_fora_do_mundo_x() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 100, 1, 2, 3);

    assert(equalInts(CRIAR_FORA_DO_MUNDO, resultado));
    assert(equalInts(0, retangulos.total));
}

void test_criar_fora_do_mundo_y() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 100, 2, 3);

    assert(equalInts(CRIAR_FORA_DO_MUNDO, resultado));
    assert(equalInts(0, retangulos.total));
}

void test_cria_retangulo_sem_largura() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 0, 3);

    assert(equalInts(CRIAR_TAMANHO_INVALIDO, resultado));
    assert(equalInts(0, retangulos.total));
}

void test_cria_retangulo_sem_altura() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 3, 0);

    assert(equalInts(CRIAR_TAMANHO_INVALIDO, resultado));
    assert(equalInts(0, retangulos.total));
}

void test_criar_sobreposto() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(criaRetangulo(&retangulos, 1, 1, 2, 3) == CRIAR_OK);

    const ResultadoCriar resultado = criaRetangulo(&retangulos, 2, 2, 2, 3);

    assert(equalInts(CRIAR_COLISAO, resultado));
    assert(equalInts(1, retangulos.total));
}


void test_apagar() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(criaRetangulo(&retangulos, 4, 1, 2, 3) == CRIAR_OK);

    const ResultadoApagar resultado = apagaRetangulo(&retangulos, 5, 3);

    assert(equalInts(APAGAR_OK, resultado));
    assert(equalInts(0, retangulos.total));
}

void test_apagar_inexistente() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    const ResultadoApagar resultado = apagaRetangulo(&retangulos, 2, 11);

    assert(equalInts(APAGAR_INEXISTENTE, resultado));
}

void test_gravidade_quando_se_apaga_o_de_cima_cai() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 10, 3, 1);
    assert(equalInts(2, retangulos.total));

    const ResultadoApagar resultado = apagaRetangulo(&retangulos, 1, 1); // apaga o de baixo

    assert(equalInts(APAGAR_OK, resultado));
    assert(equalInts(1, retangulos.total));
    assert(retangulosIguais((Retangulo) {1, 1, 3, 1},retangulos.lista[0]));
}

// MOVER
void test_mover_retangulo_esquerda() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 7, 1, 2, 3);

    const ResultadoMover resultado = moveRetangulo(&retangulos, 8, 1, -5);

    assert(equalInts(MOVER_OK, resultado));
    assert(retangulosIguais((Retangulo) {2, 1, 2, 3},retangulos.lista[0]));
}

void test_mover_retangulo_direita() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 5);

    assert(equalInts(MOVER_OK, resultado));
    assert(retangulosIguais((Retangulo) {6, 1, 2, 3},retangulos.lista[0]));
}

void test_move_com_outro_no_caminho() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(equalInts(CRIAR_OK, criaRetangulo(&retangulos, 1, 1, 2, 2)));
    assert(equalInts(CRIAR_OK, criaRetangulo(&retangulos, 10, 1, 2, 2)));

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 15);

    assert(equalInts(MOVER_COLISAO, resultado));
}

void test_gravidade_quando_sai_de_baixo_o_primeiro_cai() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 10, 3, 1);
    assert(equalInts(4, retangulos.lista[1].y)); // começa em 4

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 35); // move o debaixo para a direita

    assert(equalInts(MOVER_OK, resultado));
    assert(retangulosIguais((Retangulo) {36, 1, 2, 3},retangulos.lista[0]));
    assert(retangulosIguais((Retangulo) {1, 1, 3, 1},retangulos.lista[1]));
}

// MOVER. ERROS
void test_mover_retangulo_direita_maximo() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 1, 1);

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 79);

    assert(equalInts(MOVER_OK, resultado));
    assert(retangulosIguais((Retangulo) {80, 1, 1, 1},retangulos.lista[0]));
}

void test_mover_retangulo_esquerda_maximo() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 80, 1, 1, 1);

    const ResultadoMover resultado = moveRetangulo(&retangulos, 80, 1, -79);

    assert(equalInts(MOVER_OK, resultado));
    assert(retangulosIguais((Retangulo) {1, 1, 1, 1},retangulos.lista[0]));
}

void test_move_fora_do_mundo_esq() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 1, 3);

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, -1);

    assert(equalInts(MOVER_FORA_DO_MUNDO, resultado));
    assert(retangulosIguais((Retangulo) {1, 1, 1, 3},retangulos.lista[0]));
}

void test_move_fora_do_mundo_dir() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 80);

    assert(equalInts(MOVER_FORA_DO_MUNDO, resultado));
    assert(retangulosIguais((Retangulo) {1, 1, 2, 3},retangulos.lista[0]));
}

void test_move_sobreposto() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(equalInts( criaRetangulo(&retangulos, 1, 1, 2, 3), CRIAR_OK));
    assert(equalInts( criaRetangulo(&retangulos, 5, 1, 2, 3), CRIAR_OK));

    const ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 4);

    assert(equalInts(MOVER_COLISAO, resultado));
    assert(retangulosIguais((Retangulo) {1, 1, 2, 3},retangulos.lista[0]));
}

// FUNDIR
void test_fundir() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 4, 2, 3);

    const ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(equalInts(FUNDIR_OK, resultado));
    assert(equalInts(1, retangulos.total));
}

void test_fundir_invalido_largura_diferente() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 10, 3, 3);

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(equalInts(FUNDIR_FUSAO_INVALIDA, resultado));
    assert(equalInts(2, retangulos.total));
}

void test_fundir_invalido_diferentes_x() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 10, 1, 2, 3);

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 10, 1);

    assert(equalInts(FUNDIR_FUSAO_INVALIDA, resultado));
    assert(equalInts(2, retangulos.total));
}

void test_fundir_ret1_inexistente() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(equalInts(FUNDIR_RET1_INEXISTENTE, resultado));
}

void test_fundir_ret2_inexistente() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(equalInts(FUNDIR_RET2_INEXISTENTE, resultado));
}

void test_fundir_invalido_retangulo_no_meio() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(!criaRetangulo(&retangulos, 10, 1, 3, 3));
    assert(!criaRetangulo(&retangulos, 10, 4, 3, 3));
    assert(!criaRetangulo(&retangulos, 10, 7, 3, 3));

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 10, 1, 10, 7);

    assert(equalInts(FUNDIR_FUSAO_INVALIDA, resultado));
    assert(equalInts(3, retangulos.total));
}

void test_listar_fusoes_possiveis() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(!criaRetangulo(&retangulos, 10, 1, 3, 3));
    assert(!criaRetangulo(&retangulos, 11, 4, 3, 3)); // deslocado x+1
    FusoesPossiveis fusoesPossiveis = {0};
    listaFusoesPossiveis(&retangulos, &fusoesPossiveis);
    assert(equalInts(0, fusoesPossiveis.total));
    assert(equalInts(0, moveRetangulo(&retangulos, 11, 4, -1))); // possível agora

    listaFusoesPossiveis(&retangulos, &fusoesPossiveis);

    assert(equalInts(1, fusoesPossiveis.total));
    free(fusoesPossiveis.lista);
}

// LIMPAR
void test_limpar() {
    Retangulos retangulos = {.total = 0, .maxX = 80, .maxY = 25};
    assert(equalInts(0, retangulos.total));
    assert(NULL == retangulos.lista);
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    assert(retangulos.total);
    assert(retangulos.lista);

    limpaRetangulos(&retangulos);

    assert(equalInts(0, retangulos.total));
    assert(NULL == retangulos.lista);
}

////////////////////////////////////////////////////////////

int main() {
    // criar
    test_cria_retangulo();
    test_maximo_retangulo();
    test_minimo_retangulo();
    test_criar_cai_noutro();
    test_criar_cai_na_base();
    // criar com erros
    test_criar_fora_do_mundo_x();
    test_criar_fora_do_mundo_y();
    test_criar_sobreposto();
    test_cria_retangulo_sem_altura();
    test_cria_retangulo_sem_largura();

    // mover
    test_mover_retangulo_esquerda();
    test_mover_retangulo_direita();
    test_mover_retangulo_direita_maximo();
    test_mover_retangulo_esquerda_maximo();
    test_move_sobreposto();
    // mover com erros
    test_move_com_outro_no_caminho();
    test_move_fora_do_mundo_esq();
    test_move_fora_do_mundo_dir();
    test_gravidade_quando_sai_de_baixo_o_primeiro_cai();

    // apagar
    test_apagar();
    test_apagar_inexistente();
    test_gravidade_quando_se_apaga_o_de_cima_cai();

    // listar fusoes
    test_listar_fusoes_possiveis();

    // fundir
    test_fundir();
    test_fundir_invalido_largura_diferente();
    test_fundir_invalido_diferentes_x();
    test_fundir_invalido_retangulo_no_meio();
    test_fundir_ret1_inexistente();
    test_fundir_ret2_inexistente();

    // limpar
    test_limpar();

    return 0;
}
