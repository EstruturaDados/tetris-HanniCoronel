#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo; // Tipo da peça (I, O, T, L)
    int id;    // Identificador único
} Peca;

#define MAX 5

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;
    Peca nova;
    nova.tipo = tipos[indice];
    nova.id = id;
    return nova;
}

// Insere uma nova peça na fila
void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possivel inserir nova peça.\n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

// Remove a peça da frente da fila
void remover(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("Fila vazia. Não é possivel remover.\n");
        return;
    }

    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

// Exibe o conteúdo da fila
void mostrarFila(Fila *f) {
    printf("Fila de Peças: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

// Função principal
int main() {
    Fila f;
    inicializarFila(&f);
    srand(time(NULL));

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < MAX; i++) {
        inserir(&f, gerarPeca(i));
    }

    mostrarFila(&f);

    // Remove uma peça (simulando "jogar a peça")
    Peca jogada;
    remover(&f, &jogada);
    printf("Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);

    mostrarFila(&f);

    // Insere uma nova peça no final da fila
    Peca nova = gerarPeca(5);
    inserir(&f, nova);
    printf("Nova peça inserida: [%c %d]\n", nova.tipo, nova.id);

    mostrarFila(&f);

    return 0;
}

