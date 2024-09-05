// -------------------------------------------------------------------------------------------------
// Nombre del integrante: Pablo Daniel Barillas Moreno. Carné No. 22193
// Nombre del integrante: André Emilio Pivaral López. Carné No. 23574
// Universidad: Universidad del Valle de Guatemala
// Versión: 1.0
// Fecha: 05/09/2024
// Curso: Programación de microprocesadores
// Programa: Creación Secuencial de Hilos con Pthreads
// Descripción: Este programa utiliza la biblioteca de Pthreads para crear hilos que imprimen un 
//              mensaje de "Hello world" junto con su identificador de hilo. A diferencia de una 
//              ejecución concurrente, cada hilo se crea y luego el programa espera que termine 
//              usando `pthread_join` antes de crear el siguiente. Esto asegura que los mensajes se 
//              impriman de forma secuencial, en el mismo orden en que los hilos fueron creados. 
//              El programa crea un total de 10 hilos y asegura la sincronización adecuada entre 
//              ellos, sin permitir que se ejecuten en paralelo.
// -------------------------------------------------------------------------------------------------


#include <pthread.h>   // Biblioteca para trabajar con hilos (Pthreads).
#include <stdio.h>     // Biblioteca estándar para entrada/salida.
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
    pthread_t thread;  // Variable para almacenar el identificador del hilo.
    int rc, t, param;  // 'rc' para el código de retorno, 't' como contador, y 'param' para el argumento del hilo.

    // Ciclo para crear un hilo, hacer join y luego continuar
    for (t = 0; t < NUM_THREADS; t++) {
        printf("In main: creating thread %d\n", t);  // Imprime un mensaje indicando que se está creando un hilo.
        param = t;  // Asigna el valor del contador 't' como argumento para identificar el hilo.
        rc = pthread_create(&thread, NULL, PrintHello, (void *)&param);  // Crea un nuevo hilo y pasa 'param' como argumento.
        if (rc) {  // Si la creación del hilo falla, 'rc' será un valor distinto de 0.
            printf("ERROR; return code from pthread_create() is %d\n", rc);  // Imprime un mensaje de error si falla la creación del hilo.
            exit(-1);  // Sale del programa si hay un error en la creación del hilo.
        }
        pthread_join(thread, NULL);  // Espera a que el hilo termine su ejecución antes de crear el siguiente hilo.
    }

    return 0;  // Termina el programa principal.
}