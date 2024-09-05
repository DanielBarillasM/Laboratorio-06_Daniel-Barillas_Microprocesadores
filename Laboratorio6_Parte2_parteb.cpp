// -------------------------------------------------------------------------------------------------
// Nombre del integrante: Pablo Daniel Barillas Moreno. Carné No. 22193
// Nombre del integrante: André Emilio Pivaral López. Carné No. 23574
// Universidad: Universidad del Valle de Guatemala
// Versión: 1.0
// Fecha: 05/09/2024
// Curso: Programación de microprocesadores
// Programa: Creación y Sincronización de Hilos con Pthreads
// Descripción: Este programa utiliza la biblioteca de Pthreads para crear múltiples hilos de manera 
//              concurrente. Cada hilo imprime un mensaje que incluye su número de identificación. 
//              Se define un número fijo de hilos (10), y el programa los crea, asignando un ID a 
//              cada uno mediante un parámetro. Los hilos son sincronizados usando `pthread_join` 
//              para asegurar que el programa espere la finalización de todos los hilos antes de 
//              terminar su ejecución.
// -------------------------------------------------------------------------------------------------


#include <pthread.h>   // Biblioteca para trabajar con hilos (Pthreads).
#include <stdio.h>     // Biblioteca estándar de entrada/salida.
#include <stdlib.h>    // Biblioteca estándar para funciones como exit().

#define NUM_THREADS 10 // Define una constante para el número de hilos a crear.

void *PrintHello(void *paramID) {
    int *id;
    id = (int *)paramID;  // Castea el argumento a un puntero a entero, que será el ID del hilo.
    printf("Hello world thread No. %d!\n", *id);  // Imprime un mensaje que incluye el ID del hilo.
    pthread_exit(NULL);   // Termina el hilo y libera sus recursos.
    return NULL;          // Esta línea nunca se ejecuta, pero está aquí para cumplir con la convención de retorno de la función.
}

int main(int argc, char *argv[]) {
    pthread_t threadsID[NUM_THREADS];  // Array para almacenar los identificadores de los hilos.
    int rc, t, param[NUM_THREADS];     // 'rc' para el código de retorno, 't' como contador, y 'param' para los argumentos de los hilos.

    // Ciclo para crear los hilos
    for (t = 0; t < NUM_THREADS; t++) {
        printf("In main: creating thread %d\n", t);  // Imprime un mensaje indicando que se está creando un hilo.
        param[t] = t;  // Asigna el valor del contador 't' como argumento para identificar el hilo.
        rc = pthread_create(&threadsID[t], NULL, PrintHello, (void *)&param[t]);  // Crea un nuevo hilo y pasa 'param[t]' como argumento.
        if (rc) {  // Si la creación del hilo falla, 'rc' será un valor distinto de 0.
            printf("ERROR; return code from pthread_create() is %d\n", rc);  // Imprime un mensaje de error.
            exit(-1);  // Sale del programa si hay un error en la creación del hilo.
        }
    }

    // Ciclo separado para hacer join con cada hilo
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threadsID[t], NULL);  // Espera a que cada hilo termine antes de continuar.
    }

    return 0;  // Termina el programa principal.
}