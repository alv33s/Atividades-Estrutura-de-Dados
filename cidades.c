#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"


int compararCidades(const void *a, const void *b) {
    Cidade *cidadeA = (Cidade *)a;
    Cidade *cidadeB = (Cidade *)b;
    return cidadeA->Posicao - cidadeB->Posicao;
}


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

    if (fscanf(file, "%d", &(estrada->T)) != 1) {
        fprintf(stderr, "Erro ao ler o comprimento da estrada\n");
        free(estrada);
        fclose(file);
        return NULL;
    }
    if (estrada->T < 3 || estrada->T > 1000000) {
        fprintf(stderr, "Comprimento da estrada fora dos limites permitidos\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d", &(estrada->N)) != 1) {
        fprintf(stderr, "Erro ao ler o número de cidades\n");
        free(estrada);
        fclose(file);
        return NULL;
    }
    if (estrada->N < 2 || estrada->N > 10000) {
        fprintf(stderr, "Número de cidades fora dos limites permitidos\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        fprintf(stderr, "Erro ao alocar memória para cidades\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    char linha[512];
    fgets(linha, sizeof(linha), file); 

    for (int i = 0; i < estrada->N; i++) {
        if (!fgets(linha, sizeof(linha), file)) {
            fprintf(stderr, "Erro ao ler os dados da cidade na linha %d\n", i + 3); 
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }

        char *token = strtok(linha, " ");
        if (!token || sscanf(token, "%d", &(estrada->C[i].Posicao)) != 1) {
            fprintf(stderr, "Erro ao ler a posição da cidade na linha %d\n", i + 3);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }

        char *nome = strtok(NULL, "\n");
        if (!nome) {
            fprintf(stderr, "Erro ao ler o nome da cidade na linha %d\n", i + 3);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }

        strncpy(estrada->C[i].Nome, nome, sizeof(estrada->C[i].Nome) - 1);
        estrada->C[i].Nome[sizeof(estrada->C[i].Nome) - 1] = '\0';

        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            fprintf(stderr, "Posição da cidade %d fora dos limites permitidos\n", estrada->C[i].Posicao);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }
        for (int j = 0; j < i; j++) {
            if (estrada->C[i].Posicao == estrada->C[j].Posicao) {
                fprintf(stderr, "Posições duplicadas encontradas para as cidades\n");
                free(estrada->C);
                free(estrada);
                fclose(file);
                return NULL;
            }
        }
    }

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    fclose(file);
    return estrada;
}


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

    char *resultado = (char *)malloc((strlen(cidadeMenor) + 1) * sizeof(char));
    if (resultado) {
        strcpy(resultado, cidadeMenor);
    }

    free(estrada->C);
    free(estrada);
    return resultado;
}

int main() {
    const char *nomeArquivo = "teste1.txt";

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    if (menorVizinhanca != -1) {
        printf("Menor vizinhanca: %.2f\n", menorVizinhanca);
    } else {
        printf("Erro ao calcular a menor vizinhanca.\n");
    }

    char *cidade = cidadeMenorVizinhanca(nomeArquivo);
    if (cidade) {
        printf("Cidade com menor vizinhanca: %s\n", cidade);
        free(cidade);
    } else {
        printf("Erro ao encontrar a cidade com a menor vizinhanca.\n");
    }

    return 0;
}
