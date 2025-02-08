#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"
#include "util.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.edgelist>\n", argv[0]);
        return 1;
    }

    // Início da contagem de tempo
    clock_t inicio = clock();

    const char* nome_entrada = argv[1];
    char nome_saida[256];
    gerar_nome_saida(nome_entrada, nome_saida);

    FILE* arquivo_entrada = fopen(nome_entrada, "r");
    if (!arquivo_entrada) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    ListaAdjacencia grafo[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        inicializar_lista(&grafo[i]);
    }

    int origem, destino;
    while (fscanf(arquivo_entrada, "%d %d", &origem, &destino) == 2) {
        adicionar_vizinho(&grafo[origem], destino);
        adicionar_vizinho(&grafo[destino], origem);
    }
    fclose(arquivo_entrada);

    FILE* arquivo_saida = fopen(nome_saida, "w");
    if (!arquivo_saida) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = i + 1; j < MAX_VERTICES; j++) {
            if (grafo[i].qtd > 0 && grafo[j].qtd > 0) {
                int comuns = contar_comuns(&grafo[i], &grafo[j]);
                if (comuns > 0) {
                    fprintf(arquivo_saida, "%d %d %d\n", i, j, comuns);
                    fflush(arquivo_saida); 
                }
            }
        }
    }

    fclose(arquivo_saida);
    liberar_grafo(grafo, MAX_VERTICES);

    // Fim da contagem de tempo
    clock_t fim = clock();
    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Resultado salvo em %s\n", nome_saida);
    printf("Tempo de execução: %.3f segundos\n", tempo_execucao);

    return 0;
}
