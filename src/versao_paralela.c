#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "util.h"

#define MAX_BUFFER_SIZE 2000000

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Uso correto: %s <arquivo.edgelist>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    double tempo_inicio = MPI_Wtime();
    const char* arquivo_entrada = argv[1];

    ListaAdjacencia grafo[MAX_VERTICES];
    int* buffer = NULL;
    int buffer_size = 0;

    for (int i = 0; i < MAX_VERTICES; i++) {
        inicializar_lista(&grafo[i]);
    }

    if (rank == 0) {
        FILE* entrada = fopen(arquivo_entrada, "r");
        if (!entrada) {
            perror("Erro ao abrir arquivo");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int origem, destino;
        while (fscanf(entrada, "%d %d", &origem, &destino) == 2) {
            adicionar_vizinho(&grafo[origem], destino);
            adicionar_vizinho(&grafo[destino], origem);
        }
        fclose(entrada);

        buffer = malloc(MAX_BUFFER_SIZE * sizeof(int));
        int pos = 0;

        for (int i = 0; i < MAX_VERTICES; i++) {
            buffer[pos++] = grafo[i].qtd;
            for (int j = 0; j < grafo[i].qtd; j++) {
                buffer[pos++] = grafo[i].vizinhos[j];
            }
        }
        buffer_size = pos;

        for (int i = 1; i < size; i++) {
            MPI_Send(&buffer_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(buffer, buffer_size, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&buffer_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buffer = malloc(buffer_size * sizeof(int));
        MPI_Recv(buffer, buffer_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int pos = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        grafo[i].qtd = buffer[pos++];
        if (grafo[i].qtd > 0) {
            grafo[i].vizinhos = malloc(grafo[i].qtd * sizeof(int));
            for (int j = 0; j < grafo[i].qtd; j++) {
                grafo[i].vizinhos[j] = buffer[pos++];
            }
        }
    }

    int pares_por_processo = (MAX_VERTICES + size - 1) / size;
    int inicio = rank * pares_por_processo;
    int fim = (inicio + pares_por_processo > MAX_VERTICES) ? MAX_VERTICES : inicio + pares_por_processo;

    char nome_saida[256];
    sprintf(nome_saida, "output/saida_parcial_%d.cng", rank);
    FILE* saida = fopen(nome_saida, "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        FILE* saida_local;
        char nome_saida_thread[256];

        sprintf(nome_saida_thread, "output/saida_parcial_%d_%d.cng", rank, tid);
        saida_local = fopen(nome_saida_thread, "w");

        if (!saida_local) {
            perror("Erro ao criar arquivo de saída local");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        #pragma omp for schedule(dynamic)
        for (int i = inicio; i < fim; i++) {
            for (int j = i + 1; j < MAX_VERTICES; j++) {
                if (grafo[i].qtd > 0 && grafo[j].qtd > 0) {
                    int comuns = contar_comuns(&grafo[i], &grafo[j]);
                    if (comuns > 0) {
                        fprintf(saida_local, "%d %d %d\n", i, j, comuns);
                    }
                }
            }
        }

        fclose(saida_local);
    }

    fclose(saida);
    free(buffer);
    double tempo_fim = MPI_Wtime();

    if (rank == 0) {
        printf("Tempo total de execução: %.3f segundos\n", tempo_fim - tempo_inicio);
    }

    MPI_Finalize();
    return 0;
}
