#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "retangulos.h"

bool retangulosIguais(const Retangulo esp, const Retangulo atual)
{
    bool result = esp.x == atual.x && esp.y == atual.y && esp.l == atual.l && esp.h == atual.h;
    if (!result)
    {
        printf("esperado: ");
        imprimeRetangulo(esp);
        printf("atual:    ");
        imprimeRetangulo(atual);
    }
    return result;
}
////////////////////////////////////////////////////////////
// CRIAR
void test_criaRetangulo()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 1, 1, 2, 3);

    assert(0 == resultado);
    assert(1 == retangulos.total);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_maximoRetangulo()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 1, 1, 80, 25);

    assert(0 == resultado);
    assert(1 == retangulos.total);
    assert(retangulosIguais((Retangulo){1, 1, 80, 25}, retangulos.lista[0]));
}

void test_minimoRetangulo()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 80, 25, 1, 1); // deixa cair do topo à direita

    assert(0 == resultado);
    assert(1 == retangulos.total);
    assert(retangulosIguais((Retangulo){80, 1, 1, 1}, retangulos.lista[0]));
}

// CRIAR. ERROS
void test_criarForaDoMundoX()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 100, 1, 2, 3);

    assert(FORA_DO_MUNDO == resultado);
    assert(0 == retangulos.total);
}
void test_criarForaDoMundoY()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 1, 100, 2, 3);

    assert(FORA_DO_MUNDO == resultado);
    assert(0 == retangulos.total);
}

void test_criaRetanguloSemLargura()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 1, 1, 0, 3);

    assert(TAMANHO_INVALIDO == resultado);
    assert(0 == retangulos.total);
}

void test_criaRetanguloSemAltura()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 1, 1, 3, 0);

    assert(TAMANHO_INVALIDO == resultado);
    assert(0 == retangulos.total);
}

void test_criarSobreposto()
{
    Retangulos retangulos = {};
    int res = criaRetangulo(&retangulos, 1, 1, 2, 3);
    assert(0 == res);

    int resultado = criaRetangulo(&retangulos, 2, 2, 2, 3);

    assert(COLISAO == resultado);
    assert(1 == retangulos.total);
}

// MOVER
void test_moveRetanguloEsquerda()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 7, 1, 2, 3);

    int resultado = moveRetangulo(&retangulos, 8, 1, -5);

    assert(0 == resultado);
    assert(retangulosIguais((Retangulo){2, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveRetanguloDireita()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    int resultado = moveRetangulo(&retangulos, 1, 1, 5);

    assert(0 == resultado);
    assert(retangulosIguais((Retangulo){6, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveRetanguloDireitaMaximo()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 1, 1, 1, 1);

    int resultado = moveRetangulo(&retangulos, 1, 1, 79);

    assert(0 == resultado);
    assert(retangulosIguais((Retangulo){80, 1, 1, 1}, retangulos.lista[0]));
}

void test_moveRetanguloEsquerdaMaximo()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 80, 1, 1, 1);

    int resultado = moveRetangulo(&retangulos, 80, 1, -79);

    assert(0 == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 1, 1}, retangulos.lista[0]));
}

void test_moveForaDoMundoEsq()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    int resultado = moveRetangulo(&retangulos, 1, 1, -5);

    assert(FORA_DO_MUNDO == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveForaDoMundoDir()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    int resultado = moveRetangulo(&retangulos, 1, 1, 80);

    assert(FORA_DO_MUNDO == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_moveSobreposto()
{
    Retangulos retangulos = {};
    assert(!criaRetangulo(&retangulos, 1, 1, 2, 3));
    assert(!criaRetangulo(&retangulos, 5, 1, 2, 3));

    int resultado = moveRetangulo(&retangulos, 1, 1, 4);

    assert(COLISAO == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

// GRAVIDADE
void test_gravidade()
{
    Retangulos retangulos = {};

    int resultado = criaRetangulo(&retangulos, 1, 5, 2, 3);

    assert(0 == resultado);
    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
}

void test_gravidadeCaiEmCimaDeOutro()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 1, 1, 2, 3);

    criaRetangulo(&retangulos, 1, 10, 2, 1);

    assert(retangulosIguais((Retangulo){1, 1, 2, 3}, retangulos.lista[0]));
    assert(4 == retangulos.lista[1].y); // caiu de 10 para 4
}

void test_gravidadeQuandoSaiDeBaixoOPrimeiroCai()
{
    Retangulos retangulos = {};
    criaRetangulo(&retangulos, 1, 1, 2, 3);
    criaRetangulo(&retangulos, 1, 10, 3, 1);
    assert(4 == retangulos.lista[1].y); // começa em 4

    int resultado = moveRetangulo(&retangulos, 1, 4, 20); // move o debaixo para a direita

    assert(0 == resultado);
    assert(retangulosIguais((Retangulo){21, 1, 2, 3}, retangulos.lista[0]));
    assert(1 == retangulos.lista[1].y); // mas caiu para 1
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

    return 0;
}
