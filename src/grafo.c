#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

void inicializar_lista(ListaAdjacencia* lista) {
    lista->capacidade = 10;
    lista->qtd = 0;
    lista->vizinhos = (int*)malloc(lista->capacidade * sizeof(int));
}

void adicionar_vizinho(ListaAdjacencia* lista, int vizinho) {
    if (lista->qtd == lista->capacidade) {
        lista->capacidade *= 2;
        lista->vizinhos = (int*)realloc(lista->vizinhos, lista->capacidade * sizeof(int));
    }
    lista->vizinhos[lista->qtd++] = vizinho;
}

int contem(int* lista, int tamanho, int valor) {
    for (int i = 0; i < tamanho; i++) {
        if (lista[i] == valor) {
            return 1;
        }
    }
    return 0;
}

int contar_comuns(ListaAdjacencia* u, ListaAdjacencia* v) {
    int contador = 0;
    for (int i = 0; i < u->qtd; i++) {
        if (contem(v->vizinhos, v->qtd, u->vizinhos[i])) {
            contador++;
        }
    }
    return contador;
}

void liberar_grafo(ListaAdjacencia grafo[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(grafo[i].vizinhos);
    }
}
