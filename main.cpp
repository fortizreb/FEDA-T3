#include "usuario.h"
#include "arbolBinario.h"
#include "hashAbierto.h"
#include "hashCerrado.h"
#include "lecturaData.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
using namespace std;

// Animación de carga (esta función la saqué de un ejemplo en internet y la adapté)
void animacionCarga(atomic<bool>& corriendo) {
    int puntos = 1;
    while (corriendo) {
        cout << "\rCargando usuarios desde data.csv (hay que esperar un poco)";
        for (int j = 0; j < puntos; ++j) cout << ".";
        cout << "   "; // Espacios para limpiar
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
        puntos = (puntos % 3) + 1;
    }
    //cout << "\rListo!      " << endl;
}

int main() {

    
    auto inicio = chrono::high_resolution_clock::now();
    
    // inicio de animacuion
    atomic<bool> corriendo(true);
    thread hiloAnimacion(animacionCarga, ref(corriendo));
 


    // Leer usuarios desde el archivo CSV
    vector<Usuario> usuarios = leerUsuariosDesdeCsv("data.csv");
    
    // Detener animación 
    corriendo = false;
    hiloAnimacion.join();
    //cout << "\r" << endl;
    cout << "\rCarga de usuarios y estructuras listas para pruebas.             ";
    cout << endl;


    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion = fin - inicio;
    cout << "Tiempo de lectura: " << duracion.count() << " segundos." << endl;
    return 0;
}
