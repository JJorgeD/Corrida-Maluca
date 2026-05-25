#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// Dimensoes da pista
#define LARGURA 40
#define ALTURA  20
#define NUM_FAIXAS 4

// Structs
typedef struct {
    int x, y;       // posicao na pista
    int velocidade; // metros percorridos
    int pecas;      // pecas coletadas
} Jogador;

// Funcoes
void desenharPista();
void desenharJogador(Jogador *j);

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(100); // 100ms por frame

    Jogador jogador;
    jogador.x = NUM_FAIXAS / 2;
    jogador.y = ALTURA - 2;
    jogador.velocidade = 1;
    jogador.pecas = 0;

    int ch = 0;

    while (ch != 10) { // 10 = Enter para sair
        if (keyhit()) {
            ch = readch();
        }

        if (timerTimeOver() == 1) {
            screenClear();
            desenharPista();
            desenharJogador(&jogador);
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}

void desenharPista() {
    screenSetColor(WHITE, DARKGRAY);
    for (int i = 0; i < ALTURA; i++) {
        screenGotoxy(0, i);
        printf("|");
        screenGotoxy(LARGURA, i);
        printf("|");
    }
}

void desenharJogador(Jogador *j) {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(j->x * (LARGURA / NUM_FAIXAS) + 2, j->y);
    printf("[P]");
}
void desenharHUD(Jogador *j, int frames_mensagem, char *mensagem) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(38, 1);
    printf("Distancia: %d m", j->distancia);
    screenGotoxy(38, 2);
    printf("Velocidade: %d", j->velocidade);
    screenGotoxy(38, 3);
    printf("Pecas: %d", j->pecas);
    screenGotoxy(38, 5);
    printf("A/D: mover");
    screenGotoxy(38, 6);
    printf("Enter: sair");

    if (frames_mensagem > 0) {
        screenSetColor(YELLOW, DARKGRAY);
        screenGotoxy(5, 8);
        printf("%s", mensagem);
    }
}
void desenharCarros(Carro *lista) {
    Carro *atual = lista;
    while (atual != NULL) {
        screenSetColor(RED, DARKGRAY);
        screenGotoxy(atual->faixa * LARGURA_FAIXA + 4, atual->y);
        printf("[X]");
        atual = atual->prox;
    }
}
