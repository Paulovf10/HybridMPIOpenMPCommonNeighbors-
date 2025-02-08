#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

void gerar_nome_saida(const char* entrada, char* saida) {
    const char* pasta_saida = "output/";

    mkdir(pasta_saida, 0777);

    const char* nome_arquivo = strrchr(entrada, '/');
    if (nome_arquivo) {
        nome_arquivo++; 
    } else {
        nome_arquivo = entrada;
    }


    char nome_sem_extensao[256];
    strcpy(nome_sem_extensao, nome_arquivo);
    char* ponto = strrchr(nome_sem_extensao, '.');
    if (ponto) {
        *ponto = '\0'; 
    }
    sprintf(saida, "%s%s.cng", pasta_saida, nome_sem_extensao);
}
