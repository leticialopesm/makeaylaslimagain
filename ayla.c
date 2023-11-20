#include <stdio.h>

// Matriz 1 de Ayla
char aylaMatrix1[6][30] = {
    "          /\\--/\\ ",
    "          ( ● ● ) ",
    "           ((^))  ",
    "         (  ayla  )",
    "            U  U ",
};

// Matriz 2 de Ayla
char aylaMatrix2[6][30] = {
    "         /\\--/\\ ",
    "         ( ● ● ) ",
    "          ((^))  ",
    "         ( ayla )",
    "           U  U ",
};


void imprimirMatriz(char matriz[6][30]) {
    for (int i = 0; i < 6; i++) {
        printf("%s\n", matriz[i]);
    }
}

int main() {
    // Imprimir as matrizes
    printf("Matriz 1 de Ayla:\n");
    imprimirMatriz(aylaMatrix1);

    printf("\nMatriz 2 de Ayla:\n");
    imprimirMatriz(aylaMatrix2);

    return 0;
}
