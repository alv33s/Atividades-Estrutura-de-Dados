# Atividades-Estrutura-de-Dados

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// Função para inicializar as cidades a partir do arquivo
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        fprintf(stderr, "Erro ao alocar memória para Estrada\n");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &(estrada->T)); // Lê o comprimento da estrada
    fscanf(file, "%d", &(estrada->N)); // Lê o número de cidades

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        fprintf(stderr, "Erro ao alocar memória para cidades\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        fscanf(file, "%d %s", &(estrada->C[i].Posicao), estrada->C[i].Nome);
    }

    fclose(file);
    return estrada;
}

// Função para calcular a menor vizinhança
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1;

    double menorVizinhanca = estrada->T;
    for (int i = 0; i < estrada->N; i++) {
        double inicioVizinhanca = (i == 0) ? 0 : (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
        double fimVizinhanca = (i == estrada->N - 1) ? estrada->T : (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        double vizinhanca = fimVizinhanca - inicioVizinhanca;
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

// Função para retornar o nome da cidade com a menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    double menorVizinhanca = estrada->T;
    char *cidadeMenor = NULL;
    for (int i = 0; i < estrada->N; i++) {
        double inicioVizinhanca = (i == 0) ? 0 : (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
        double fimVizinhanca = (i == estrada->N - 1) ? estrada->T : (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        double vizinhanca = fimVizinhanca - inicioVizinhanca;
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            cidadeMenor = estrada->C[i].Nome;
        }
    }

    // Copia o nome da cidade com a menor vizinhança
    char *resultado = (char *)malloc((strlen(cidadeMenor) + 1) * sizeof(char));
    if (resultado) {
        strcpy(resultado, cidadeMenor);
    }

    free(estrada->C);
    free(estrada);
    return resultado;
}

printf(getEstrada());
printf(calcularMenorVizinhanca());
printf(cidadeMenorVizinhanca());
