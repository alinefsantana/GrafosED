#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NOS 200

typedef struct {
    int matriz[MAX_NOS][MAX_NOS];
    int total_nos;
} Grafo;

void adicionaLigacao(Grafo* g, int origem, int destino, int peso) {
    g->matriz[origem][destino] = peso;
    g->matriz[destino][origem] = peso;
}

void inicializaGrafo(Grafo* g, int total_nos) {
    g->total_nos = total_nos;
    for (int i = 0; i < total_nos; i++) {
        for (int j = 0; j < total_nos; j++) {
            g->matriz[i][j] = 0;
        }
    }
}

void buscaLargura(Grafo* g, int origem, int destino) {
    bool visitado[MAX_NOS] = {false};
    int fila[MAX_NOS], inicio_fila = 0, fim_fila = 0;
    int caminho[MAX_NOS];

    for (int i = 0; i < g->total_nos; i++) caminho[i] = -1;

    fila[fim_fila++] = origem;
    visitado[origem] = true;

    while (inicio_fila < fim_fila) {
        int no = fila[inicio_fila++];

        for (int i = 0; i < g->total_nos; i++) {
            int ligacao = g->matriz[no][i];
            if (ligacao > 0 && !visitado[i]) {
                fila[fim_fila++] = i;
                visitado[i] = true;
                caminho[i] = no;

                printf("Saiu do vértice %d com peso %d para o vértice %d\n", no, ligacao, i);

                if (i == destino) break;
            }
        }
    }

    if (!visitado[destino]) {
        printf("Não há caminho entre %d e %d.\n", origem, destino);
    } else {
        printf("Caminho encontrado: ");
        int trajeto[MAX_NOS], tam = 0;
        for (int no = destino; no != -1; no = caminho[no])
            trajeto[tam++] = no;

        for (int i = tam - 1; i >= 0; i--)
            printf("%d ", trajeto[i]);
        printf("\n");
    }
}

void buscaProfundidade(Grafo* g, int origem) {
    bool explorado[MAX_NOS] = {false};
    int pilha[MAX_NOS], topo = -1;

    pilha[++topo] = origem;

    while (topo >= 0) {
        int no = pilha[topo--];

        if (!explorado[no]) {
            printf("Visitando vértice %d\n", no);
            explorado[no] = true;
        }

        for (int i = g->total_nos - 1; i >= 0; i--) {
            int ligacao = g->matriz[no][i];
            if (ligacao > 0 && !explorado[i]) {
                pilha[++topo] = i;
                printf("Saiu do vértice %d com peso %d para o vértice %d\n", no, ligacao, i);
            }
        }
    }
    printf("\n");
}

void carregaArquivo(Grafo* g, const char* arquivo) {
    FILE* file = fopen(arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int total_nos;
    fscanf(file, "%d", &total_nos);
    inicializaGrafo(g, total_nos);

    for (int i = 0; i < total_nos; i++) {
        for (int j = 0; j < total_nos; j++) {
            int peso;
            fscanf(file, "%d", &peso);
            adicionaLigacao(g, i, j, peso);
        }
    }

    fclose(file);
}

int main() {
    Grafo g;
    int opcao, origem, destino;

    carregaArquivo(&g, "pcv50.txt");

    while (1) {
        printf("\n1. BFS\n2. DFS\n3. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o vértice de origem e destino: ");
                scanf("%d %d", &origem, &destino);
                buscaLargura(&g, origem, destino);
                break;
            case 2:
                printf("Digite o vértice inicial para DFS: ");
                scanf("%d", &origem);
                buscaProfundidade(&g, origem);
                break;
            case 3:
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
