#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura da peça
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente, tras, tamanho;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------- Funções auxiliares ----------
char gerarTipoAleatorio() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca(int *contador) {
    Peca nova;
    nova.tipo = gerarTipoAleatorio();
    nova.id = (*contador)++;
    return nova;
}

// ---------- Fila Circular ----------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == MAX_FILA;
}

void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = nova;
    f->tamanho++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

Peca frenteFila(Fila *f) {
    if (filaVazia(f)) return (Peca){'-', -1};
    return f->itens[f->frente];
}

// ---------- Pilha ----------
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

Peca pop(Pilha *p) {
    if (pilhaVazia(p)) return (Peca){'-', -1};
    return p->itens[p->topo--];
}

Peca topoPilha(Pilha *p) {
    if (pilhaVazia(p)) return (Peca){'-', -1};
    return p->itens[p->topo];
}

// ---------- Exibição ----------
void exibirEstado(Fila *f, Pilha *p) {
    printf("=== Estado Atual ===\n");
    printf("Fila de Pecas: ");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");

    printf("Pilha de Reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n\n");
}

// ---------- Ações ----------
void jogarPeca(Fila *f, int *contador) {
    if (filaVazia(f)) {
        printf("Nenhuma peça para jogar!\n");
        return;
    }
    Peca jogada = desenfileirar(f);
    printf("Peça [%c %d] jogada da fila.\n", jogada.tipo, jogada.id);

    // Gera nova peça para manter a fila cheia
    Peca nova = gerarPeca(contador);
    enfileirar(f, nova);
}

void reservarPeca(Fila *f, Pilha *p, int *contador) {
    if (filaVazia(f)) return;
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Não é possível reservar.\n");
        return;
    }
    Peca frente = desenfileirar(f);
    push(p, frente);
    printf("Peça [%c %d] movida da fila para a pilha.\n", frente.tipo, frente.id);

    Peca nova = gerarPeca(contador);
    enfileirar(f, nova);
}

void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhuma peça reservada para usar.\n");
        return;
    }
    Peca usada = pop(p);
    printf("Peça [%c %d] usada da pilha.\n", usada.tipo, usada.id);
}

void trocarPecas(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Impossível trocar — estruturas vazias.\n");
        return;
    }
    Peca temp = topoPilha(p);
    p->itens[p->topo] = frenteFila(f);
    f->itens[f->frente] = temp;
    printf("Peça da frente trocada com o topo da pilha.\n");
}

void trocaMultipla(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("Troca múltipla não permitida. Faltam peças.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        Peca temp = f->itens[idx];
        f->itens[idx] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// ---------- Programa Principal ----------
int main() {
    Fila fila;
    Pilha pilha;
    int contador = 0, opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(&contador));
    }

    // Inicializa a pilha com 3 peças
    for (int i = 0; i < MAX_PILHA; i++) {
        push(&pilha, gerarPeca(&contador));
    }

    do {
        printf("\n=====================================\n");
        printf("        Tetris        \n");
        printf("=====================================\n\n");

        exibirEstado(&fila, &pilha);

        printf("Opções:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para reserva (pilha)\n");
        printf("3 - Usar peca da reserva (pilha)\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: jogarPeca(&fila, &contador); break;
            case 2: reservarPeca(&fila, &pilha, &contador); break;
            case 3: usarPecaReservada(&pilha); break;
            case 4: trocarPecas(&fila, &pilha); break;
            case 5:
                trocaMultipla(&fila, &pilha);
                printf("\n=== Novo Estado ===\n");
                exibirEstado(&fila, &pilha);
                break;
            case 0: printf("Encerrando o jogo...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

