/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

struct Objeto {
    char nome[50];
    double x;
    double y;
    double incX;
    double incY;
};

float obterIncrementoAleatorio() {
    // Gera um número aleatório para determinar o incremento
    srand(time(0));
    float aleatorio = rand() % 120 + 1;

    float incremento;

    // Calcula o incremento com base no valor aleatório gerado
    if (aleatorio >= 10)
        incremento = aleatorio / 1000;
    else
        incremento = aleatorio / 100;

    // Define a direção do incremento com base no valor aleatório
    if (aleatorio <= 50)
        return incremento;
    else
        return -incremento;
}

void imprimirObjeto(double proximoX, double proximoY, char* objeto, int cor) {
    // Define a cor e posição na tela para imprimir um objeto
    screenSetColor(cor, DARKGRAY);
    screenGotoxy(proximoX, proximoY);
    printf("%s", objeto);
}

void limparObjetos() {
    // Limpa os objetos na tela para atualização
    for (int i = 9; i < MAXY; i++) {
        screenGotoxy(MINX + 1, i);
        for (int j = 0; j < 77; j++) {
            printf(" ");
        }
    }
}

void imprimirPontuacao(int pontos) {
    // Imprime a pontuação na tela
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 4);
    printf("PONTUAÇÃO: %d ", pontos);
}

int main() {
    static int tecla = 0;
    int pontos = 0, margemX = 5, margemY = 0, colisao = 0;
    double gravidade = 0.22;

    // Inicialização dos objetos do jogo
    struct Objeto ayla;
    ayla.x = 20.0;
    ayla.y = MAXY - 1;
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

    while (tecla != 10) // Enter
    {
        if (keyhit() && ayla.y >= MAXY - 3) {
            tecla = readch();
            imprimirPontuacao(pontos);
            screenUpdate();
        }

        // Atualizar estado do jogo
        if (timerTimeOver() == 1) {

            // Limpar tela
            limparObjetos();

            // Movimento dos objetos
            ayla.y = ayla.y + ayla.incY;
            obstaculoInferior.x = obstaculoInferior.x + obstaculoInferior.incX;
            obstaculoSuperior.x = obstaculoSuperior.x + obstaculoSuperior.incX;

            // Gravidade
            ayla.incY = ayla.incY + gravidade;

            // Pulo da Ayla
            if (tecla == 32 && ayla.y >= MAXY - 1) ayla.incY = -2.0, tecla = 0;

            // Chão
            if (ayla.y >= MAXY - 1) ayla.y = MAXY - 1;
            if (obstaculoInferior.y >= MAXY - 1) obstaculoInferior.y = MAXY - 1;
            if (obstaculoSuperior.y >= MAXY - 2) obstaculoSuperior.y = MAXY - 2;

            // Loop de obstáculos
            float incrementoAleatorio = obterIncrementoAleatorio();
            if (obstaculoInferior.x <= MINX + 1) obstaculoInferior.x = MAXX - 8, pontos++, obstaculoInferior.incX += incrementoAleatorio;
            if (obstaculoSuperior.x <= MINX + 1) obstaculoSuperior.x = MAXX - 8, obstaculoSuperior.incX += incrementoAleatorio;

            // Colisões
            if ((abs((int)ayla.x - (int)obstaculoInferior.x) <= margemX && abs((int)ayla.y - (int)obstaculoInferior.y) <= margemY) || (abs((int)ayla.x - (int)obstaculoSuperior.x) <= margemX && abs((int)ayla.y - (int)obstaculoSuperior.y) <= margemY)) {
                colisao = 1;

                obstaculoInferior.incX = 0;
                obstaculoSuperior.incX = 0;

                ayla.incY = 0;
                gravidade = 0;

                if (abs((int)ayla.y - (int)obstaculoSuperior.y) <= margemY) ayla.y = obstaculoSuperior.y - 1;
                else if (abs((int)ayla.x - (int)obstaculoInferior.x) <= margemX) ayla.x = obstaculoInferior.x - 5;
            }

            // GAME OVER
            if (colisao == 1) {
                imprimirObjeto(35, 12, "GAME OVER", 1);
            }

            // Recomeçar
            if (obstaculoInferior.incX == 0 && tecla == 114) {
                colisao = 0;

                obstaculoInferior.incX = -1.0;
                obstaculoSuperior.incX = -1.0;

                ayla.incY = 0;
                gravidade = 0.2;

                obstaculoInferior.x = 60;
                obstaculoSuperior.x = 60;

                ayla.y = MAXY - 1;

                pontos = 0;
                tecla = 0;
            }

            // Impressões
            imprimirPontuacao(pontos);

            imprimirObjeto(ayla.x, ayla.y, "Ayla", 6);

            imprimirObjeto(obstaculoInferior.x, obstaculoInferior.y, "|||", 2);
            imprimirObjeto(obstaculoSuperior.x, obstaculoSuperior.y, "|||", 2);

            // Atualização da tela
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
