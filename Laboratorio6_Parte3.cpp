// -------------------------------------------------------------------------------------------------
// Nombre: Pablo Daniel Barillas Moreno
// Universidad: Universidad del Valle de Guatemala
// Versión: 1.0
// Fecha: 05/09/2024
// Curso: Programación de microprocesadores
// Programa: Cálculo Paralelo de Series con Pthreads
// Descripción: Este programa utiliza la biblioteca de Pthreads para calcular de manera paralela la 
//              suma de una serie alternante definida por (-1)^(n+1) / n para valores de n positivos. 
//              El usuario ingresa un valor máximo de n, y el programa crea un hilo para calcular 
//              cada término de la serie. Se asegura la protección del acceso a la suma total usando 
//              un mutex, y se imprime el resultado al final de la ejecución.
// -------------------------------------------------------------------------------------------------

#include <iostream>
#include <pthread.h>
#include <vector>
#include <mutex>
#include <cmath> // Para std::pow
#include <limits> // Para std::numeric_limits

// Estructura para pasar parámetros a los hilos
struct ThreadArgs {
    int n;
    double* result;
    std::mutex* mtx;
};

// Función para calcular un término de la serie
void* calculateTerm(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    int n = threadArgs->n;
    double term = std::pow(-1, n + 1) / n; // Calcular el término (-1)^(n+1) / n

    // Sumar el término a la suma total de forma segura usando un mutex
    std::lock_guard<std::mutex> lock(*(threadArgs->mtx));
    *(threadArgs->result) += term;

    return nullptr;
}

int main() {
    int maxN;

    // Bucle para asegurar que se obtenga un número entero positivo válido
    while (true) {
        std::cout << "\nIngrese el valor máximo de n a evaluar en la serie: ";
        
        // Validar la entrada del usuario
        if (!(std::cin >> maxN)) {
            std::cerr << "\nEntrada no válida. Por favor, ingrese un número entero positivo." << std::endl;
            std::cin.clear(); // Limpiar el estado de error de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada no válida
            continue; // Volver al inicio del bucle para solicitar una nueva entrada
        }

        // Validar que maxN sea positivo
        if (maxN > 0) {
            break; // Salir del bucle si se obtiene un número entero positivo válido
        } else {
            std::cerr << "\nEl valor de n debe ser un número entero positivo. Intente de nuevo." << std::endl;
        }
    }

    double sum = 0.0; // Variable para almacenar la suma de la serie
    std::mutex mtx; // Mutex para proteger el acceso a la suma
    std::vector<pthread_t> threads(maxN); // Vector para almacenar los identificadores de los hilos
    std::vector<ThreadArgs> args(maxN); // Vector para almacenar los argumentos de los hilos

    // Crear un hilo para cada término de la serie
    for (int i = 1; i <= maxN; ++i) {
        args[i - 1].n = i;
        args[i - 1].result = &sum;
        args[i - 1].mtx = &mtx;
        pthread_create(&threads[i - 1], nullptr, calculateTerm, &args[i - 1]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < maxN; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Imprimir la suma total de la serie
    std::cout << "\nLa suma de la serie hasta n = " << maxN << " es: " << sum << std::endl << "\n";

    return 0;
}
