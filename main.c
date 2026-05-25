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

Carro* criarCarro(int faixa, int y, int direcao) {
    Carro *novo = (Carro*) malloc(sizeof(Carro));
    novo->faixa = faixa;
    novo->y = y;
    novo->direcao = direcao;
    novo->prox = NULL;
    return novo;
}
void adicionarCarro(Carro **lista, int faixa, int y, int direcao) {
    Carro *novo = criarCarro(faixa, y, direcao);
    novo->prox = *lista;
    *lista = novo;
}
void verificarVelocidade(Jogador *j, int frames, int *frames_mensagem, char *mensagem) {
    int nova_velocidade = j->velocidade;

    if      (frames >= 450 && j->velocidade < 10) { nova_velocidade = 10; strcpy(mensagem, "*** VELOCIDADE MAXIMA! ***"); }
    else if (frames >= 400 && j->velocidade < 9)  { nova_velocidade = 9;  strcpy(mensagem, "*** VELOCIDADE 9! ***"); }
    else if (frames >= 350 && j->velocidade < 8)  { nova_velocidade = 8;  strcpy(mensagem, "*** VELOCIDADE 8! ***"); }
    else if (frames >= 300 && j->velocidade < 7)  { nova_velocidade = 7;  strcpy(mensagem, "*** VELOCIDADE 7! ***"); }
    else if (frames >= 250 && j->velocidade < 6)  { nova_velocidade = 6;  strcpy(mensagem, "*** VELOCIDADE 6! ***"); }
    else if (frames >= 200 && j->velocidade < 5)  { nova_velocidade = 5;  strcpy(mensagem, "*** VELOCIDADE 5! ***"); }
    else if (frames >= 150 && j->velocidade < 4)  { nova_velocidade = 4;  strcpy(mensagem, "*** VELOCIDADE 4! ***"); }
    else if (frames >= 100 && j->velocidade < 3)  { nova_velocidade = 3;  strcpy(mensagem, "*** VELOCIDADE 3! ***"); }
    else if (frames >= 50  && j->velocidade < 2)  { nova_velocidade = 2;  strcpy(mensagem, "*** VELOCIDADE 2! ***"); }

    if (nova_velocidade > j->velocidade) {
        j->velocidade = nova_velocidade;
        *frames_mensagem = 25;
    }
}
