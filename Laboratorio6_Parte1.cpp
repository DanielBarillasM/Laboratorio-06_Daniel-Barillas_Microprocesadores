// -------------------------------------------------------------------------------------------------
// Nombre del integrante: Pablo Daniel Barillas Moreno. Carné No. 22193
// Nombre del integrante: André Emilio Pivaral López. Carné No. 23574
// Universidad: Universidad del Valle de Guatemala
// Versión: 1.0
// Fecha: 05/09/2024
// Curso: Programación de microprocesadores
// Programa: Cálculo Paralelo de Números de Fibonacci con Pthreads
// Descripción: Este programa utiliza la biblioteca de Pthreads para calcular de manera paralela los 
//              números de Fibonacci en un rango dado por el usuario. El programa solicita un número 
//              entre 0 y 100, y luego crea un hilo que calcula todos los números de Fibonacci hasta 
//              el número indicado. El programa asegura la exclusión mutua en el acceso a los datos 
//              utilizando un mutex. Finalmente, se imprimen los números de Fibonacci encontrados y 
//              se calcula su suma total.
// -------------------------------------------------------------------------------------------------

#include <iostream>   // Incluye la biblioteca estándar para entrada y salida.
#include <pthread.h>  // Incluye la biblioteca de Pthreads para trabajar con hilos.
#include <vector>     // Incluye la clase vector de C++ para usar contenedores dinámicos.
#include <mutex>      // Incluye la biblioteca para manejo de mutex (exclusión mutua).
#include <limits>     // Incluye para std::numeric_limits, usado para limpiar el buffer de entrada.

// Estructura para pasar parámetros a los hilos
struct FibonacciArgs {
    int maxNumber;                   // Número máximo hasta el cual se evaluará la serie de Fibonacci.
    std::vector<int> fibonacciNumbers; // Vector para almacenar los números de Fibonacci encontrados.
    std::mutex* mtx;                 // Puntero a un mutex para proteger el acceso concurrente al vector.
};

// Función para calcular los números de Fibonacci
void* calculateFibonacci(void* args) {
    FibonacciArgs* fibArgs = (FibonacciArgs*)args; // Cast del argumento a la estructura FibonacciArgs.
    int maxNumber = fibArgs->maxNumber;            // Extrae el número máximo de la estructura.
    std::vector<int>& fibonacciNumbers = fibArgs->fibonacciNumbers; // Referencia al vector de números de Fibonacci.
    std::mutex* mtx = fibArgs->mtx;                // Referencia al mutex para protección de datos.

    // Evaluar cada número desde 0 hasta maxNumber
    for (int num = 0; num <= maxNumber; ++num) {   // Itera a través de cada número en el rango [0, maxNumber].
        int a = 0, b = 1;                          // Inicializa los dos primeros números de la serie de Fibonacci.

        // Comprobar si el número actual es un número de Fibonacci
        while (a <= num) {                         // Genera números de Fibonacci hasta que 'a' exceda 'num'.
            if (a == num) {                        // Si 'a' coincide con 'num', entonces 'num' es un número de Fibonacci.
                std::lock_guard<std::mutex> lock(*mtx); // Bloquea el mutex para acceso exclusivo al vector.
                fibonacciNumbers.push_back(num);   // Añade 'num' al vector de números de Fibonacci.
                break;                             // Sale del bucle una vez encontrado el número de Fibonacci.
            }
            int next = a + b;                      // Calcula el siguiente número de Fibonacci.
            a = b;                                 // Actualiza 'a' al siguiente número en la serie.
            b = next;                              // Actualiza 'b' al siguiente número en la serie.
        }
    }

    return nullptr;                                // Retorna nullptr para indicar la terminación del hilo.
}

int main() {
    int maxNumber; // Variable para almacenar el número máximo ingresado por el usuario.

    // Bucle infinito para asegurar que se obtenga un número válido
    while (true) {
        std::cout << "\nIngrese un número entre 0 y 100: "; // Solicita al usuario un número en el rango [0, 100].

        // Validar la entrada del usuario
        if (!(std::cin >> maxNumber)) {             // Verifica si la entrada es un número válido.
            std::cerr << "\nEntrada no válida. Por favor, ingrese un número entero." << std::endl; // Mensaje de error si la entrada es inválida.
            std::cin.clear();                       // Limpia el estado de error de 'cin'.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora la entrada no válida.
            continue;                               // Reinicia el bucle para solicitar una nueva entrada.
        }

        // Validar el rango del número ingresado
        if (maxNumber >= 0 && maxNumber <= 100) {   // Comprueba si el número está en el rango permitido.
            break;                                  // Sale del bucle si la entrada es válida.
        } else {
            std::cerr << "\nEl número debe estar entre 0 y 100. Intente de nuevo." << std::endl; // Mensaje de error para entrada fuera de rango.
        }
    }

    // Inicializar los parámetros y la estructura para los hilos
    FibonacciArgs fibArgs;                           // Crea una instancia de la estructura para pasar a los hilos.
    fibArgs.maxNumber = maxNumber;                   // Asigna el número máximo a la estructura.
    std::mutex mtx;                                  // Crea un mutex para proteger el acceso al vector.
    fibArgs.mtx = &mtx;                              // Asigna la dirección del mutex a la estructura.

    // Crear el hilo
    pthread_t thread;                                // Declara una variable para el identificador del hilo.
    pthread_create(&thread, nullptr, calculateFibonacci, &fibArgs); // Crea un nuevo hilo que ejecuta 'calculateFibonacci'.

    // Esperar a que el hilo termine
    pthread_join(thread, nullptr);                   // Espera a que el hilo termine su ejecución antes de continuar.

    // Calcular la suma de los números de Fibonacci encontrados
    int sum = 0;                                     // Inicializa la suma de los números de Fibonacci.
    std::cout << "\nNúmeros de Fibonacci en el rango 0 hasta " << maxNumber << ":" << std::endl << "\n"; // Imprime encabezado.
    for (size_t i = 0; i < fibArgs.fibonacciNumbers.size(); ++i) { // Itera a través de los números de Fibonacci encontrados.
        std::cout << "F" << i << " = " << fibArgs.fibonacciNumbers[i] << std::endl; // Imprime cada número de Fibonacci.
        sum += fibArgs.fibonacciNumbers[i];       // Suma cada número de Fibonacci a 'sum'.
    }

    // Imprimir la suma total
    std::cout << "\nSuma total de los números de Fibonacci: " << sum << std::endl << "\n"; // Imprime la suma total.

    return 0; // Termina el programa principal.
}
