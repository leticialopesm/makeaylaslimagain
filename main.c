#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAXY 24
#define MINX 1
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

typedef struct node {
  int pontmax;
  struct node *next;
} lista;

// Função para adicionar um nó de forma descendente
void addnode(lista **head, int var) {
  lista *n = *head;
  lista *novo = (lista *)malloc(sizeof(lista));

  novo->pontmax = var;
  novo->next = NULL;

  if (*head == NULL) {
    *head = novo;
  } else if (n->pontmax < var) {
    novo->next = *head;
    *head = novo;
  } else {
    while (n->next != NULL && n->next->pontmax > var) {
      n = n->next;
    }
    if (n->next == NULL) {
      novo->next = NULL;
      n->next = novo;
    } else {
      novo->next = n->next;
      n->next = novo;
    }
  }
}

// Função para imprimir a pontuação máxima
void printHighScore(lista *head, int score) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(30, 6);
  printf("PONTUAÇÃO MÁXIMA: "); // Texto alterado aqui

  // Ajusta a posição do cursor para depois dos dois pontos
  screenGotoxy(30 + strlen("PONTUAÇÃO MÁXIMA: "), 6);

  if (head == NULL || score > head->pontmax)
    printf("%d", score);
  else
    printf("%d", head->pontmax);
}

void desenharAyla(double x, double y, int pontos) {
  y -= 0;
  screenSetColor(YELLOW, YELLOW);

  if (pontos >= 14) {
    screenGotoxy(x, y);
    printf("          /\\---/\\ ");
    screenGotoxy(x, y + 1);
    printf("          ( ● ● ) ");
    screenGotoxy(x, y + 2);
    printf("           ((^))  ");
    screenGotoxy(x, y + 3);
    printf("           (   )");
    screenGotoxy(x, y + 4);
    printf("            U  U ");   
  }
  else if (pontos >= 7){
    screenGotoxy(x, y);
      printf("          /\\---/\\ ");
      screenGotoxy(x, y + 1);
      printf("          ( ● ● ) ");
      screenGotoxy(x, y + 2);
      printf("           ((^))  ");
      screenGotoxy(x, y + 3);
      printf("           (    )");
      screenGotoxy(x, y + 4);
      printf("            U  U ");
  } else {
    screenGotoxy(x, y);
    printf("          /\\---/\\ ");
    screenGotoxy(x, y + 1);
    printf("          ( ● ● ) ");
    screenGotoxy(x, y + 2);
    printf("           ((^))  ");
    screenGotoxy(x, y + 3);
    printf("          (      )");
    screenGotoxy(x, y + 4);
    printf("            U  U ");
  }
}

void limparObjetos() {
  for (int i = 0; i < MAXY; i++) {
    screenGotoxy(MINX + 1, i);
    for (int j = 0; j < MAXX - 2; j++) {
      printf(" ");
    }
  }
}

void imprimirPontuacao(int pontos, lista *head) {
  screenSetColor(LIGHTMAGENTA, DARKGRAY);
  screenGotoxy(35, 4);
  printf("PONTUAÇÃO: %d ", pontos);
  printHighScore(head, pontos);
}

int main() {
  static int tecla = 0;
  int pontos = 0, margemX = 5, margemY = 0, colisao = 0;
  double gravidade = 0.23;
  lista *head = NULL; // Lista para o high score

  struct Objeto ayla;
  ayla.x = 20.0;
  ayla.y = MAXY - 5;
  ayla.incY = 0;

  struct Objeto obstaculoInferior;
  obstaculoInferior.x = 60;
  obstaculoInferior.y = MAXY - 1;
  obstaculoInferior.incX = -1.0;

  struct Objeto obstaculoSuperior;
  obstaculoSuperior.x = 60;
  obstaculoSuperior.y = MAXY - 2;
  obstaculoSuperior.incX = -1.0;

  screenInit(1);
  keyboardInit();
  timerInit(50);
  screenUpdate();

  while (tecla != 10) { // Enter
    if (keyhit()) {
      tecla = readch();
      if (tecla == 32 && ayla.y >= MAXY - 6)
        ayla.incY = -2.0; // Lógica do pulo
    }

    if (colisao) {
      addnode(&head, pontos); // Atualiza a lista de high score
      screenSetColor(LIGHTMAGENTA, DARKGRAY);
      screenGotoxy(35, 12);
      printf("GAME OVER");
      getchar();
      screenUpdate();
      break; // Encerra o loop principal e termina o jogo
    }

    if (timerTimeOver()) {
      limparObjetos();

      ayla.y += ayla.incY;
      obstaculoInferior.x += obstaculoInferior.incX;
      obstaculoSuperior.x += obstaculoSuperior.incX;

      if (ayla.y < MAXY - 5)
        ayla.incY += gravidade;
      else
        ayla.incY = 0;

      if (ayla.y > MAXY - 5)
        ayla.y = MAXY - 5;

      if (obstaculoInferior.x <= MINX + 1) {
        obstaculoInferior.x = MAXX - 8;
        pontos++;
      }
      if (obstaculoSuperior.x <= MINX + 1)
        obstaculoSuperior.x = MAXX - 8;

      if ((abs((int)ayla.x - (int)obstaculoInferior.x) <= margemX &&
           abs((int)ayla.y - (int)obstaculoInferior.y) <= margemY) ||
          (abs((int)ayla.x - (int)obstaculoSuperior.x) <= margemX &&
           abs((int)ayla.y - (int)obstaculoSuperior.y) <= margemY)) {
        colisao = 1;
        obstaculoInferior.incX = 0;
        obstaculoSuperior.incX = 0;
        ayla.incY = 0;
        gravidade = 0;
      } else {
        imprimirPontuacao(pontos, head);
        desenharAyla(ayla.x, ayla.y, pontos);

        screenSetColor(LIGHTGREEN, DARKGRAY);
        screenGotoxy(obstaculoInferior.x, obstaculoInferior.y);
        printf("|||");
        screenGotoxy(obstaculoSuperior.x, obstaculoSuperior.y);
        printf("|||");

        screenUpdate();
      }
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
