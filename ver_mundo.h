#ifndef VER_MUNDO_H
#define VER_MUNDO_H

#define PINTADO_CONTORNO 'X'
#define PINTADO_INTERIOR 'O'
#define VAZIO ' '

void imprimeMundo(const Retangulos *retangulos);
void imprimeFusoesPossiveis(const Retangulos *retangulos);
void imprimeListaRetangulos(const Retangulos *retangulos);

#endif
