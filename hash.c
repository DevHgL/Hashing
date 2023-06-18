#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int matricula;
    char nome[50];
    char curso[50];
} Aluno;

void inicializarTabela(Aluno tabela[], int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        tabela[i].matricula = -1; // -1 indica posição vazia
    }
}

int calcularHash(int matricula, int tamanho) {
    return matricula % tamanho;
}

int proximaPosicaoLivre(int posicao, int tamanho) {
    return (posicao + 1) % tamanho;
}

void inserirAluno(Aluno tabela[], int tamanho) {
    FILE *arquivo;
    Aluno aluno;
    int matricula, posicao, i;

    arquivo = fopen("alunos.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Digite a matricula do aluno: ");
    scanf("%d", &matricula);

    // Verifica se a matrícula já existe na tabela
    for (i = 0; i < tamanho; i++) {
        if (tabela[i].matricula == matricula) {
            printf("Matricula ja existente! Por favor, insira outra matricula.\n");
            fclose(arquivo);
            return;
        }
    }

    printf("Digite o nome do aluno: ");
    scanf(" %49[^\n]", aluno.nome);

    printf("Digite o curso do aluno: ");
    scanf(" %49[^\n]", aluno.curso);

    aluno.matricula = matricula;

    posicao = calcularHash(matricula, tamanho);

    if (tabela[posicao].matricula == -1) {
        tabela[posicao] = aluno;
        fwrite(&aluno, sizeof(Aluno), 1, arquivo);
        printf("Aluno inserido com sucesso!\n");
    } else {
        // Tratamento de colisão: próxima posição livre
        i = proximaPosicaoLivre(posicao, tamanho);
        while (i != posicao && tabela[i].matricula != -1) {
            i = proximaPosicaoLivre(i, tamanho);
        }
        if (i == posicao) {
            printf("Erro: tabela hash cheia!\n");
        } else {
            tabela[i] = aluno;
            fwrite(&aluno, sizeof(Aluno), 1, arquivo);
            printf("Aluno inserido com sucesso!\n");
        }
    }

    fclose(arquivo);
}

void imprimirAluno(Aluno tabela[], int tamanho) {
    FILE *arquivo;
    Aluno aluno;
    int matricula, posicao;

    arquivo = fopen("alunos.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Digite a matricula do aluno: ");
    scanf("%d", &matricula);

    posicao = calcularHash(matricula, tamanho);

    fseek(arquivo, posicao * sizeof(Aluno), SEEK_SET);
    if (fread(&aluno, sizeof(Aluno), 1, arquivo) == 1) {
        if (aluno.matricula == matricula) {
            printf("Aluno encontrado:\n");
            printf("Matricula: %d\n", aluno.matricula);
            printf("Nome: %s\n", aluno.nome);
            printf("Curso: %s\n", aluno.curso);
            fclose(arquivo);
            return;
        } else {
            // Verifica se o aluno está em alguma posição posterior devido à colisão
            int i = proximaPosicaoLivre(posicao, tamanho);
            while (i != posicao) {
                fseek(arquivo, i * sizeof(Aluno), SEEK_SET);
                if (fread(&aluno, sizeof(Aluno), 1, arquivo) == 1) {
                    if (aluno.matricula == matricula) {
                        printf("Aluno encontrado:\n");
                        printf("Matricula: %d\n", aluno.matricula);
                        printf("Nome: %s\n", aluno.nome);
                        printf("Curso: %s\n", aluno.curso);
                        fclose(arquivo);
                        return;
                    }
                }
                i = proximaPosicaoLivre(i, tamanho);
            }
        }
    }

    printf("Aluno nao encontrado!\n");
    fclose(arquivo);
}

void imprimirTabela(Aluno tabela[], int tamanho) {
    FILE *arquivo;
    Aluno aluno;
    int i;

    arquivo = fopen("alunos.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Tabela de dispersao:\n");
    for (i = 0; i < tamanho; i++) {
        fseek(arquivo, i * sizeof(Aluno), SEEK_SET);
        if (fread(&aluno, sizeof(Aluno), 1, arquivo) == 1) {
            printf("Posicao %d:\n", i);
            if (aluno.matricula == -1) {
                printf("Vazia\n");
            } else {
                printf("Matricula: %d\n", aluno.matricula);
                printf("Nome: %s\n", aluno.nome);
                printf("Curso: %s\n", aluno.curso);
            }
            printf("------------------\n");
        }
    }

    fclose(arquivo);
}

int main() {
    int opcao;
    Aluno tabela[100];
    inicializarTabela(tabela, 100);

    do {
        printf("\nMenu:\n");
        printf("1. Inserir um novo aluno\n");
        printf("2. Imprimir as informacoes de um determinado aluno\n");
        printf("3. Imprimir a tabela de dispersao\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirAluno(tabela, 100);
                break;
            case 2:
                imprimirAluno(tabela, 100);
                break;
            case 3:
                imprimirTabela(tabela, 100);
                break;
            case 4:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 4);

    return 0;
}
