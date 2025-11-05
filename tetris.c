#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ------------------------------
// Funções da Fila
// ------------------------------

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->itens[f->fim] = nova;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

void desenfileirar(Fila *f, Peca *removida) {
    if (filaVazia(f)) return;
    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
}

void mostrarFila(Fila *f) {
    printf("Fila de Peças: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

// ------------------------------
// Funções da Pilha
// ------------------------------

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = nova;
}

void pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) return;
    *removida = p->itens[p->topo];
    p->topo--;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

// ------------------------------
// Geração automática de peças
// ------------------------------

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ------------------------------
// Programa principal
// ------------------------------

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idAtual = 0;
    int opcao;

    // Preenche fila inicial
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    do {
        printf("\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        printf("\n");

        printf("1 - Jogar Peça\n");
        printf("2 - Reservar Peça\n");
        printf("3 - Usar Peça Reservada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Jogar peça: remove da fila e adiciona nova
            Peca jogada;
            desenfileirar(&fila, &jogada);
            enfileirar(&fila, gerarPeca(idAtual++));
        }
        else if (opcao == 2) {
            // Reservar peça: da fila para a pilha
            if (pilhaCheia(&pilha)) {
                printf("Pilha cheia! Não é possível reservar.\n");
            } else {
                Peca reservada;
                desenfileirar(&fila, &reservada);
                push(&pilha, reservada);
                enfileirar(&fila, gerarPeca(idAtual++));
            }
        }
        else if (opcao == 3) {
            // Usar peça da pilha
            if (pilhaVazia(&pilha)) {
                printf("Pilha vazia! Nenhuma peça reservada.\n");
            } else {
                Peca usada;
                pop(&pilha, &usada);
            }
        }

    } while (opcao != 0);

    printf("\nEncerrando o jogo...\n");
    return 0;
}

