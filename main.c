#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MAXY 25
#define MINX 0
#define MAXX 80
#define DARKGRAY 8
#define YELLOW 14
#define LIGHTMAGENTA 13
#define LIGHTGREEN 10

struct Objeto {
    double x;
    double y;
    double incX;
    double incY;
};

void desenharAyla(double x, double y, int pontos) {
    y -= 0;
    screenSetColor(YELLOW, DARKGRAY);

    if (pontos >= 7) {
        // DESENHO 1
        screenGotoxy(x, y);     printf("          /\\--/\\ ");
        screenGotoxy(x, y + 1); printf("          ( ● ● ) ");
        screenGotoxy(x, y + 2); printf("           ((^))  ");
        screenGotoxy(x, y + 3); printf("          ( ayla )");
        screenGotoxy(x, y + 4); printf("            U  U ");
    } else {
        // DESENHO 2
        screenGotoxy(x, y);     printf("          /\\--/\\ ");
        screenGotoxy(x, y + 1); printf("          ( ● ● ) ");
        screenGotoxy(x, y + 2); printf("           ((^))  ");
        screenGotoxy(x, y + 3); printf("         (  ayla  )");
        screenGotoxy(x, y + 4); printf("            U  U ");
    }
}

void limparObjetos() {
    // Limpa objetos na tela para atualização
    for (int i = 0; i < MAXY; i++) {
        screenGotoxy(MINX + 1, i);
        for (int j = 0; j < MAXX - 2; j++) {
            printf(" ");
        }
    }
}

void imprimirPontuacao(int pontos) {
    // Imprime a pontuação na tela com a cor LIGHTMAGENTA
    screenSetColor(LIGHTMAGENTA, DARKGRAY);
    screenGotoxy(35, 4);
    printf("PONTUAÇÃO: %d ", pontos);
}

int main() {
    static int tecla = 0;
    int pontos = 0, margemX = 5, margemY = 0, colisao = 0;
    double gravidade = 0.22;

    // Inicialização do objeto do jogo
    struct Objeto ayla;
    ayla.x = 20.0;
    ayla.y = MAXY - 5; // Ajustado para o desenho do personagem
    ayla.incY = 0;

    struct Objeto obstaculoInferior;
    obstaculoInferior.x = 60;
    obstaculoInferior.y = MAXY - 1;
    obstaculoInferior.incX = -1.0;

    struct Objeto obstaculoSuperior;
    obstaculoSuperior.x = 60;
    obstaculoSuperior.y = MAXY - 2;
    obstaculoSuperior.incX = -1.0;

    // Estado inicial do jogo
    screenInit(1);
    keyboardInit();
    timerInit(50);
    screenUpdate();

    while (tecla != 10) { // Enter
        if (keyhit()) {
            tecla = readch();
            if (tecla == 32 && ayla.y >= MAXY - 6) ayla.incY = -2.0; // Lógica do pulo
        }

        if (colisao) {
            screenSetColor(LIGHTMAGENTA, DARKGRAY);
            screenGotoxy(35, 12); printf("FIM DE JOGO");
            screenUpdate();
            while (tecla != 'r') {
                if (keyhit()) {
                    tecla = readch();
                }
            }
            // Condições de reinício após pressionar 'r'
            colisao = 0;
            obstaculoInferior.incX = -1.0;
            obstaculoSuperior.incX = -1.0;
            ayla.incY = 0;
            gravidade = 0.22;
            obstaculoInferior.x = 60;
            obstaculoSuperior.x = 60;
            ayla.y = MAXY - 5;
            pontos = 0;
            screenClear();
            screenUpdate();
            continue; // Continua para reiniciar o laço do jogo
        }

        // Atualização do estado do jogo
        if (timerTimeOver()) {
            // Limpa a tela
            limparObjetos();

            // Movimento dos objetos
            ayla.y += ayla.incY;
            obstaculoInferior.x += obstaculoInferior.incX;
            obstaculoSuperior.x += obstaculoSuperior.incX;

            // Gravidade
            if (ayla.y < MAXY - 5) ayla.incY += gravidade; // Aplica gravidade se acima do chão
            else ayla.incY = 0; // Para de cair se estiver no chão

            // Verificação do chão
            if (ayla.y > MAXY - 5) ayla.y = MAXY - 5; // Mantém Ayla no chão

            // Loop do obstáculo
            if (obstaculoInferior.x <= MINX + 1) {
                obstaculoInferior.x = MAXX - 8;
                pontos++;
            }
            if (obstaculoSuperior.x <= MINX + 1) obstaculoSuperior.x = MAXX - 8;

            // Verificação de colisão
            if ((abs((int)ayla.x - (int)obstaculoInferior.x) <= margemX && abs((int)ayla.y - (int)obstaculoInferior.y) <= margemY) || 
                (abs((int)ayla.x - (int)obstaculoSuperior.x) <= margemX && abs((int)ayla.y - (int)obstaculoSuperior.y) <= margemY)) {
                colisao = 1;
                obstaculoInferior.incX = 0;
                obstaculoSuperior.incX = 0;
                ayla.incY = 0;
                gravidade = 0;
                continue; // Pula para o início do laço para tratar a colisão
            }

            // Imprime a pontuação e Ayla
            imprimirPontuacao(pontos);
            desenharAyla(ayla.x, ayla.y, pontos);

            // Imprime obstáculos com a cor LIGHTGREEN
            screenSetColor(LIGHTGREEN, DARKGRAY);
            screenGotoxy(obstaculoInferior.x, obstaculoInferior.y); printf("|||");
            screenGotoxy(obstaculoSuperior.x, obstaculoSuperior.y); printf("|||");

            // Atualização da tela
            screenUpdate();
        }
    }

    // Limpeza
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
