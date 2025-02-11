# Projeto de Análise de Vizinhos Comuns em Grafos

Este projeto implementa uma solução para o problema de encontrar vizinhos comuns em grafos não-direcionados. A solução foi desenvolvida em duas versões: uma versão sequencial e uma versão paralela hibrida utilizando MPI (Message Passing Interface) e OpenMP.

## Descrição do Problema

Dado um grafo não-direcionado representado em formato de lista de arestas (`edgelist`), o objetivo é calcular, para cada par de vértices \(u\) e \(v\), a quantidade de vértices que são vizinhos comuns de ambos. 

Por exemplo, para o grafo representado como:

```
0 1
0 2
1 2
1 3
2 3
```

Os vizinhos comuns entre \(1\) e \(3\) são \(2\).

## Estrutura do Projeto

A estrutura do projeto é a seguinte:

```
cmpt_mpi/
├── Makefile
├── bin/
│   ├── versao_paralela
│   └── versao_sequencial
├── include/
│   ├── grafo.h
│   └── util.h
├── input/
│   ├── entrada.edgelist
│   ├── entrada_1000.edgelist
│   ├── entrada_100000.edgelist
│   ├── entrada_200000.edgelist
│   └── entrada_500000.edgelist
├── output/
│   ├── entrada.cng
│   ├── saida_parcial_0.cng
│   ├── saida_parcial_1.cng
│   ├── saida_parcial_2.cng
│   └── saida_parcial_3.cng
└── src/
    ├── grafo.c
    ├── util.c
    ├── versao_paralela.c
    └── versao_sequencial.c
```

## Funcionalidades

- **Versão Sequencial:** Calcula os vizinhos comuns utilizando um único processo.
- **Versão Paralela:** Utiliza programcao hibrida com OpenMP e MPI para dividir o trabalho entre múltiplos processos.

## Instruções de Compilação

Certifique-se de ter o 	exttt{gcc} e o 	exttt{mpicc} instalados no sistema.

1. Compile o projeto executando o comando:
   ```
   make all
   ```
   Os binários serão gerados na pasta `bin/`.

## Instruções de Execução

### Versão Sequencial

Execute a versão sequencial com o seguinte comando:
```
./bin/versao_sequencial input/entrada.edgelist
```
O resultado será salvo na pasta `output/` com o nome `entrada.cng`.

### Versão Paralela

Execute a versão paralela utilizando MPI:
```
mpirun -np <n_procs> ./bin/versao_paralela input/entrada.edgelist
```
Cada processo gerará um arquivo parcial na pasta `output/`, como `saida_parcial_0.cng`, `saida_parcial_1.cng`, etc.

## Observações

- Certifique-se de ajustar os tamanhos de buffer para grafos maiores.
