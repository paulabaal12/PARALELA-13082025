
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include <omp.h>        

#define HEIGHT 8        // Define la altura de la matriz (número de filas)
#define WIDTH 8         // Define el ancho de la matriz (número de columnas)

// Función para llenar la matriz con valores aleatorios de brillo
void llenar_matriz(int foto[HEIGHT][WIDTH]) {
    srand(time(NULL)); // Inicializa la semilla para números aleatorios usando la hora actual
    for (int i = 0; i < HEIGHT; i++) // Recorre cada fila
        for (int j = 0; j < WIDTH; j++) // Recorre cada columna
            foto[i][j] = rand() % 1000; // Asigna un valor aleatorio entre 0 y 999 a cada píxel
}

// Función para imprimir la matriz de brillo en pantalla
void imprimir_matriz(int foto[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) { // Recorre cada fila
        for (int j = 0; j < WIDTH; j++) // Recorre cada columna
            printf("%4d ", foto[i][j]); // Imprime el valor del píxel con formato
        printf("\n"); // Salto de línea
    }
}

// Parte Serial: Busca el valor máximo y sus coordenadas recorriendo toda la matriz
void max_brillo_serial(int foto[HEIGHT][WIDTH]) {
    int max_val = foto[0][0]; // Inicializa el máximo con el primer valor de la matriz
    int max_i = 0, max_j = 0; // Inicializa las coordenadas del máximo
    for (int i = 0; i < HEIGHT; i++) { // Recorre cada fila
        for (int j = 0; j < WIDTH; j++) { // Recorre cada columna
            if (foto[i][j] > max_val) { // Si el valor actual es mayor que el máximo
                max_val = foto[i][j];   // Actualiza el valor máximo
                max_i = i;              // Guarda la fila del nuevo máximo
                max_j = j;              // Guarda la columna del nuevo máximo
            }
        }
    }
    printf("Serial:\n");
    printf("Maximo valor: %d\n", max_val); // Imprime el valor máximo encontrado
    printf("Coordenadas: (%d, %d)\n", max_i, max_j); // Imprime las coordenadas del máximo
}

// Parte Paralela: Busca el valor máximo usando varios hilos con OpenMP
void max_brillo_paralelo(int foto[HEIGHT][WIDTH]) {
    int max_val = foto[0][0]; // Inicializa el máximo global
    int max_i = 0, max_j = 0; // Inicializa las coordenadas globales

    #pragma omp parallel // Inicia una región paralela
    {
        int local_max = foto[0][0]; // Cada hilo tiene su propio máximo local
        int local_i = 0, local_j = 0; // Y sus propias coordenadas locales

        #pragma omp for collapse(2) // Distribuye los bucles anidados entre los hilos
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (foto[i][j] > local_max) { // Si el valor actual es mayor que el máximo local
                    local_max = foto[i][j];   // Actualiza el máximo local
                    local_i = i;              // Guarda la fila local
                    local_j = j;              // Guarda la columna local
                }
            }
        }

        #pragma omp critical // Sección crítica: solo un hilo a la vez puede ejecutar esto
        {
            if (local_max > max_val) { // Si el máximo local es mayor que el global
                max_val = local_max;   // Actualiza el máximo global
                max_i = local_i;       // Actualiza la fila global
                max_j = local_j;       // Actualiza la columna global
            }
        }
    }

    printf("Paralelo:\n");
    printf("Maximo valor: %d\n", max_val); // Imprime el valor máximo encontrado en paralelo
    printf("Coordenadas: (%d, %d)\n", max_i, max_j); // Imprime las coordenadas del máximo
}

// Función principal del programa
int main() {
    int foto[HEIGHT][WIDTH]; // Declara la matriz de brillo
    llenar_matriz(foto);     // Llena la matriz con valores aleatorios
    printf("Matriz de brillo:\n");
    imprimir_matriz(foto);   // Imprime la matriz

    max_brillo_serial(foto);   // Llama a la función serial
    max_brillo_paralelo(foto); // Llama a la función paralela

    return 0;
}