#ifndef GRAFO_H
#define GRAFO_H

#define MAX_VERTICES 100000

typedef struct {
    int* vizinhos;
    int qtd;
    int capacidade;
} ListaAdjacencia;

void inicializar_lista(ListaAdjacencia* lista);
void adicionar_vizinho(ListaAdjacencia* lista, int vizinho);
int contar_comuns(ListaAdjacencia* u, ListaAdjacencia* v);
void liberar_grafo(ListaAdjacencia grafo[], int tamanho);

#endif
