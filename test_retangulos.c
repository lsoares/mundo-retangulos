#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "retangulos.h"

bool retangulosIguais(const Retangulo esp, const Retangulo atual)
{
    bool result = esp.x == atual.x && esp.y == atual.y && esp.l == atual.l && esp.h == atual.h;
    if (!result)
    {
        fprintf(stderr, "❌ Retângulos diferem");
        fprintf(stderr, "esperado: ");
        imprimeRetangulo(esp);
        fprintf(stderr, "atual:    ");
        imprimeRetangulo(atual);
    }
    return result;
}
////////////////////////////////////////////////////////////
// CRIAR
void test_criaRetangulo()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 2, 3);

    assert(CRIAR_OK == resultado);
    assert(1 == retangulos.total);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_maximoRetangulo()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 80, 25);

    assert(CRIAR_OK == resultado);
    assert(1 == retangulos.total);
    assert(retangulosIguais((Retangulo){1, 1, 80, 25}, retangulos.lista[0]));
}

void test_minimoRetangulo()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 80, 25, 1, 1); // deixa cair do topo à direita

    assert(CRIAR_OK == resultado);
    assert(1 == retangulos.total);
    assert(retangulosIguais((Retangulo){80, 1, 1, 1}, retangulos.lista[0]));
}

// CRIAR. ERROS
void test_criarForaDoMundoX()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 100, 1, 2, 3);

    assert(CRIAR_FORA_DO_MUNDO == resultado);
    assert(0 == retangulos.total);
}
void test_criarForaDoMundoY()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 100, 2, 3);

    assert(CRIAR_FORA_DO_MUNDO == resultado);
    assert(0 == retangulos.total);
}

void test_criaRetanguloSemLargura()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 0, 3);

    assert(CRIAR_TAMANHO_INVALIDO == resultado);
    assert(0 == retangulos.total);
}

void test_criaRetanguloSemAltura()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 1, 3, 0);

    assert(CRIAR_TAMANHO_INVALIDO == resultado);
    assert(0 == retangulos.total);
}

void test_criarSobreposto()
{
    Retangulos retangulos = {0};
    ResultadoCriar res = criaRetangulo(&retangulos, 1, 1, 2, 3);
    assert(CRIAR_OK == res);

    int resultado = criaRetangulo(&retangulos, 2, 2, 2, 3);

    assert(CRIAR_COLISAO == resultado);
    assert(1 == retangulos.total);
}

// MOVER
void test_moveRetanguloEsquerda()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 7, 1, 2, 3);

    ResultadoMover resultado = moveRetangulo(&retangulos, 8, 1, -5);

    assert(MOVER_OK == resultado);
    assert(retangulosIguais((Retangulo){2, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveRetanguloDireita()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 5);

    assert(MOVER_OK == resultado);
    assert(retangulosIguais((Retangulo){6, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveRetanguloDireitaMaximo()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 1, 1);

    int resultado = moveRetangulo(&retangulos, 1, 1, 79);

    assert(MOVER_OK == resultado);
    assert(retangulosIguais((Retangulo){80, 1, 1, 1}, retangulos.lista[0]));
}

void test_moveRetanguloEsquerdaMaximo()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 80, 1, 1, 1);

    ResultadoMover resultado = moveRetangulo(&retangulos, 80, 1, -79);

    assert(MOVER_OK == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 1, 1}, retangulos.lista[0]));
}

void test_moveForaDoMundoEsq()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 1, 3);

    ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, -1);

    assert(MOVER_FORA_DO_MUNDO == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 1, 3}, retangulos.lista[0]));
}

void test_moveForaDoMundoDir()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 80);

    assert(MOVER_FORA_DO_MUNDO == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveSobreposto()
{
    Retangulos retangulos = {0};
    assert(!criaRetangulo(&retangulos, 1, 1, 2, 3));
    assert(!criaRetangulo(&retangulos, 5, 1, 2, 3));

    ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 4);

    assert(MOVER_COLISAO == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

// GRAVIDADE
void test_gravidade()
{
    Retangulos retangulos = {0};

    ResultadoCriar resultado = criaRetangulo(&retangulos, 1, 5, 2, 3);

    assert(CRIAR_OK == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_gravidadeCaiEmCimaDeOutro()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    criaRetangulo(&retangulos, 1, 10, 2, 1);

    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
    assert(4 == retangulos.lista[1].y); // caiu de 10 para 4
}

void test_gravidadeQuandoSaiDeBaixoOPrimeiroCai()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 10, 3, 1);
    assert(4 == retangulos.lista[1].y); // começa em 4

    ResultadoMover resultado = moveRetangulo(&retangulos, 1, 1, 35); // move o debaixo para a direita

    assert(MOVER_OK == resultado);
    assert(retangulosIguais((Retangulo){36, 1, 2, 3}, retangulos.lista[0]));
    assert(retangulosIguais((Retangulo){1, 1, 3, 1}, retangulos.lista[1]));
}

void test_fundir()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 4, 2, 3);

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(FUNDIR_OK == resultado);
    assert(1 == retangulos.total);
}

void test_fundir_invalido()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 10, 1, 2, 4);

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 10, 1);

    assert(FUNDIR_FUSAO_INVALIDA == resultado);
    assert(2 == retangulos.total);
}

void test_fundir_ret1_nao_encontrado()
{
    Retangulos retangulos = {0};

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(FUNDIR_RET1_NAO_ENCONTRADO == resultado);
}

void test_fundir_ret2_nao_encontrado()
{
    Retangulos retangulos = {0};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    ResultadoFundir resultado = fundeRetangulos(&retangulos, 1, 1, 1, 4);

    assert(FUNDIR_RET2_NAO_ENCONTRADO == resultado);
}

void test_limpar()
{
    Retangulos retangulos = {0};
    assert(0 == retangulos.total);
    assert(NULL == retangulos.lista);
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    assert(retangulos.total);
    assert(retangulos.lista);

    limpaRetangulos(&retangulos);

    assert(0 == retangulos.total);
    assert(NULL == retangulos.lista);
}

////////////////////////////////////////////////////////////

int main()
{
    test_criaRetangulo();
    test_maximoRetangulo();
    test_minimoRetangulo();
    test_criarForaDoMundoX();
    test_criarForaDoMundoY();
    test_criarSobreposto();
    test_criaRetanguloSemAltura();
    test_criaRetanguloSemLargura();

    test_moveRetanguloEsquerda();
    test_moveRetanguloDireita();
    test_moveRetanguloDireitaMaximo();
    test_moveRetanguloEsquerdaMaximo();
    test_moveForaDoMundoEsq();
    test_moveForaDoMundoDir();
    test_moveSobreposto();

    test_gravidade();
    test_gravidadeCaiEmCimaDeOutro();
    test_gravidadeQuandoSaiDeBaixoOPrimeiroCai();

    test_fundir();
    test_fundir_invalido();
    test_fundir_ret1_nao_encontrado();
    test_fundir_ret2_nao_encontrado();
    test_limpar();

    return 0;
}
