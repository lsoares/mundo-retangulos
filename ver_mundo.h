#pragma once

#include "retangulos.h"

#define PINTADO_CONTORNO 'X'
#define PINTADO_INTERIOR 'O'
#define VAZIO ' '

void imprimeMundo(const Retangulos *retangulos);
void imprimeFusoesPossiveis(const Retangulos *retangulos);
void imprimeListaRetangulos(const Retangulos *retangulos);
