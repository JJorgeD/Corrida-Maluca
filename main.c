#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define LARGURA 36
#define ALTURA  18
#define NUM_FAIXAS 4
#define LARGURA_FAIXA (LARGURA / NUM_FAIXAS)
#define MAX_RANKING 5

typedef struct {
    int x, y;
    int velocidade;
    int distancia;
} Jogador;

typedef struct Carro {
    int faixa;
    int y;
    int direcao;
    struct Carro *prox;
} Carro;

typedef struct {
    char nome[50];
    int distancia;
} Entrada;

void telaInicio();
void telaGameOver(int distancia);
void telaRanking();
void salvarRanking(char *nome, int distancia);
void desenharPista();
void desenharJogador(Jogador *j);
void desenharHUD(Jogador *j, int frames_mensagem, char *mensagem);
void desenharCarros(Carro *lista);
Carro* criarCarro(int faixa, int y, int direcao);
void adicionarCarro(Carro **lista, int faixa, int y, int direcao);
void verificarVelocidade(Jogador *j, int frames, int *frames_mensagem, char *mensagem);
void moverCarros(Carro **lista);
void liberarCarros(Carro **lista);
int verificarColisao(Jogador *j, Carro *lista);

int main() {
    srand(time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(200);

    while (1) {
        telaInicio();

        Jogador jogador;
        jogador.x = 1;
        jogador.y = ALTURA - 3;
        jogador.velocidade = 1;
        jogador.distancia = 0;

        Carro *carros = NULL;
        int ch = 0;
        int rodando = 1;
        int frames = 0;
        int ultimo_carro_frame = 0;
        int frames_mensagem = 0;
        char mensagem_velocidade[30] = "";

        while (rodando) {
            if (keyhit()) {
                ch = readch();
                if (ch == 27) {
                    readch();
                    ch = readch();
                    if (ch == 68 && jogador.x > 0) jogador.x--;
                    else if (ch == 67 && jogador.x < NUM_FAIXAS - 1) jogador.x++;
                }
                if (ch == 97 && jogador.x > 0) jogador.x--;
                if (ch == 100 && jogador.x < NUM_FAIXAS - 1) jogador.x++;
                if (ch == 10) rodando = 0;
            }

            if (timerTimeOver() == 1) {
                frames++;
                jogador.distancia += jogador.velocidade;
                verificarVelocidade(&jogador, frames, &frames_mensagem, mensagem_velocidade);

                int movimentos = 1 + (jogador.velocidade / 3);
                int m;
                for (m = 0; m < movimentos; m++) {
                    if (frames % 2 == 0)
                        moverCarros(&carros);
                }

                int intervalo = 20 / jogador.velocidade;
                if (intervalo < 3) intervalo = 3;

                if (frames - ultimo_carro_frame >= intervalo) {
                    int faixa = rand() % NUM_FAIXAS;
                    adicionarCarro(&carros, faixa, 0, 1);
                    ultimo_carro_frame = frames;
                }

                if (verificarColisao(&jogador, carros)) {
                    rodando = 0;
                }

                screenClear();
                desenharPista();
                desenharCarros(carros);
                desenharJogador(&jogador);
                desenharHUD(&jogador, frames_mensagem, mensagem_velocidade);
                if (frames_mensagem > 0) frames_mensagem--;
                screenUpdate();
            }
        }

        telaGameOver(jogador.distancia);
        liberarCarros(&carros);
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}

void telaInicio() {
    int ch = 0;
    int opcao = 0;
    int atualizar = 1;

    while (1) {
        if (atualizar) {
            screenClear();
            screenSetColor(YELLOW, DARKGRAY);
            screenGotoxy(10, 4);
            printf("================================");
            screenGotoxy(10, 5);
            printf("        CORRIDA  MALUCA         ");
            screenGotoxy(10, 6);
            printf("================================");

            FILE *f = fopen("highscore.txt", "r");
            screenSetColor(CYAN, DARKGRAY);
            screenGotoxy(10, 8);
            if (f != NULL) {
                char nome[50];
                int score;
                fscanf(f, "%s %d", nome, &score);
                fclose(f);
                printf("Melhor: %s - %d metros", nome, score);
            } else {
                printf("Nenhum recorde ainda!");
            }

            screenSetColor(opcao == 0 ? YELLOW : WHITE, DARKGRAY);
            screenGotoxy(10, 11);
            printf("> JOGAR");

            screenSetColor(opcao == 1 ? YELLOW : WHITE, DARKGRAY);
            screenGotoxy(10, 12);
            printf("> VER RANKING");

            screenSetColor(opcao == 2 ? YELLOW : WHITE, DARKGRAY);
            screenGotoxy(10, 13);
            printf("> SAIR");

            screenSetColor(WHITE, DARKGRAY);
            screenGotoxy(10, 15);
            printf("W/S para navegar, Enter para selecionar");
            screenUpdate();
            atualizar = 0;
        }

        if (keyhit()) {
            ch = readch();
            atualizar = 1;
            if (ch == 27) {
                readch();
                ch = readch();
                if (ch == 65 && opcao > 0) opcao--;
                if (ch == 66 && opcao < 2) opcao++;
            }
            if (ch == 'w' && opcao > 0) opcao--;
            if (ch == 's' && opcao < 2) opcao++;
            if (ch == 10) {
                if (opcao == 0) return;
                if (opcao == 1) { telaRanking(); atualizar = 1; }
                if (opcao == 2) {
                    keyboardDestroy();
                    screenDestroy();
                    timerDestroy();
                    exit(0);
                }
            }
        }
    }
}

void telaRanking() {
    screenClear();
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(10, 3);
    printf("================================");
    screenGotoxy(10, 4);
    printf("            RANKING             ");
    screenGotoxy(10, 5);
    printf("================================");

    FILE *f = fopen("ranking.txt", "r");
    screenSetColor(WHITE, DARKGRAY);
    if (f != NULL) {
        char nome[50];
        int dist;
        int pos = 1;
        screenGotoxy(10, 7);
        printf("%-4s %-20s %s", "POS", "NOME", "DISTANCIA");
        screenGotoxy(10, 8);
        printf("--------------------------------");
        int linha = 9;
        while (fscanf(f, "%s %d", nome, &dist) == 2 && pos <= MAX_RANKING) {
            screenSetColor(pos == 1 ? YELLOW : WHITE, DARKGRAY);
            screenGotoxy(10, linha++);
            printf("%-4d %-20s %d m", pos++, nome, dist);
        }
        fclose(f);
    } else {
        screenGotoxy(10, 8);
        printf("Nenhum recorde registrado ainda!");
    }

    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(10, 16);
    printf("Pressione ENTER para voltar...");
    screenUpdate();

    int ch = 0;
    while (ch != 10) {
        if (keyhit()) ch = readch();
    }
}

void salvarRanking(char *nome, int distancia) {
    Entrada entradas[MAX_RANKING + 1];
    int total = 0;

    FILE *f = fopen("ranking.txt", "r");
    if (f != NULL) {
        while (total < MAX_RANKING && fscanf(f, "%s %d", entradas[total].nome, &entradas[total].distancia) == 2) {
            total++;
        }
        fclose(f);
    }

    strcpy(entradas[total].nome, nome);
    entradas[total].distancia = distancia;
    total++;

    int i, j;
    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - i - 1; j++) {
            if (entradas[j].distancia < entradas[j+1].distancia) {
                Entrada temp = entradas[j];
                entradas[j] = entradas[j+1];
                entradas[j+1] = temp;
            }
        }
    }

    f = fopen("ranking.txt", "w");
    if (f != NULL) {
        int limite = total < MAX_RANKING ? total : MAX_RANKING;
        for (i = 0; i < limite; i++) {
            fprintf(f, "%s %d\n", entradas[i].nome, entradas[i].distancia);
        }
        fclose(f);
    }

    f = fopen("highscore.txt", "w");
    if (f != NULL) {
        fprintf(f, "%s %d\n", entradas[0].nome, entradas[0].distancia);
        fclose(f);
    }
}

void telaGameOver(int distancia) {
    screenClear();
    screenSetColor(RED, DARKGRAY);
    screenGotoxy(10, 5);
    printf("================================");
    screenGotoxy(10, 6);
    printf("           GAME OVER!           ");
    screenGotoxy(10, 7);
    printf("================================");

    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(10, 9);
    printf("Distancia percorrida: %d metros", distancia);

    FILE *f = fopen("highscore.txt", "r");
    int record = 0;
    char nome_record[50] = "";
    if (f != NULL) {
        fscanf(f, "%s %d", nome_record, &record);
        fclose(f);
    }

    screenGotoxy(10, 10);
    if (distancia > record) {
        screenSetColor(YELLOW, DARKGRAY);
        printf("NOVO HIGH SCORE!");
    } else {
        screenSetColor(CYAN, DARKGRAY);
        printf("High Score: %s - %d metros", nome_record, record);
    }

    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(10, 12);
    printf("Digite seu nome: ");
    screenUpdate();

    char nome[50];
    int i = 0;
    int ch;
    while (i < 49) {
        if (keyhit()) {
            ch = readch();
            if (ch == 10) break;
            nome[i++] = ch;
            printf("%c", ch);
            screenUpdate();
        }
    }
    nome[i] = '\0';

    salvarRanking(nome, distancia);
}

void desenharPista() {
    screenSetColor(WHITE, DARKGRAY);
    for (int i = 0; i < ALTURA; i++) {
        screenGotoxy(0, i);
        printf("|");
        for (int f = 1; f < NUM_FAIXAS; f++) {
            screenGotoxy(f * LARGURA_FAIXA, i);
            if (i % 2 == 0) printf(":");
        }
        screenGotoxy(LARGURA, i);
        printf("|");
    }
}

void desenharJogador(Jogador *j) {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(j->x * LARGURA_FAIXA + 2, j->y);
    printf("[P]");
}

void desenharHUD(Jogador *j, int frames_mensagem, char *mensagem) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(38, 1);
    printf("Distancia: %d m", j->distancia);
    screenGotoxy(38, 2);
    printf("Velocidade: %d", j->velocidade);
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

void moverCarros(Carro **lista) {
    Carro *atual = *lista;
    Carro *anterior = NULL;

    while (atual != NULL) {
        atual->y += atual->direcao;

        if (atual->y >= ALTURA) {
            Carro *remover = atual;
            if (anterior == NULL)
                *lista = atual->prox;
            else
                anterior->prox = atual->prox;
            atual = atual->prox;
            free(remover);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

void liberarCarros(Carro **lista) {
    Carro *atual = *lista;
    while (atual != NULL) {
        Carro *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    *lista = NULL;
}

int verificarColisao(Jogador *j, Carro *lista) {
    Carro *atual = lista;
    while (atual != NULL) {
        if (atual->faixa == j->x &&
            atual->y >= j->y - 1 &&
            atual->y <= j->y + 1)
            return 1;
        atual = atual->prox;
    }
    return 0;
}