#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
#include "HashMap/HashMapList.h"
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <climits>

using namespace std;

// Estructuras globales para estadísticas
map<string, map<string, double>> ventasPorCiudadYPais;
map<string, map<string, double>> ventasPorProductoYPais;
map<string, map<string, pair<double, int>>> ventasPorCategoriaYPais;
map<string, map<string, int>> envioPorPais;
map<string, map<string, int>> envioPorCategoria;
map<string, double> ventasPorDia;
map<string, map<string, int>> estadoEnvioPorPais;
map<string, int> unidadesVendidas;

int contadorIfs = 0;

void procesarArchivo() {
    ifstream archivo("ventas_sudamerica(1).csv");
    string linea;
    char delimitador = ',';
    getline(archivo, linea); // Encabezado

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string idVenta, fecha, pais, ciudad, cliente, producto, categoria;
        string cantidadStr, precioStr, montoStr, medioEnvio, estadoEnvio;

        getline(ss, idVenta, delimitador);
        getline(ss, fecha, delimitador);
        getline(ss, pais, delimitador);
        getline(ss, ciudad, delimitador);
        getline(ss, cliente, delimitador);
        getline(ss, producto, delimitador);
        getline(ss, categoria, delimitador);
        getline(ss, cantidadStr, delimitador);
        getline(ss, precioStr, delimitador);
        getline(ss, montoStr, delimitador);
        getline(ss, medioEnvio, delimitador);
        getline(ss, estadoEnvio, delimitador);

        int cantidad = stoi(cantidadStr);
        float monto = stof(montoStr);

        ventasPorCiudadYPais[pais][ciudad] += monto;
        ventasPorProductoYPais[pais][producto] += monto;
        ventasPorCategoriaYPais[pais][categoria].first += monto;
        ventasPorCategoriaYPais[pais][categoria].second += 1;
        envioPorPais[pais][medioEnvio]++;
        envioPorCategoria[categoria][medioEnvio]++;
        ventasPorDia[fecha] += monto;
        estadoEnvioPorPais[pais][estadoEnvio]++;
        unidadesVendidas[producto] += cantidad;
    }
}

// Funciones para mostrar resultados

void mostrarTopCiudadesPorPais() {
    cout << "Top 5 ciudades con mayor monto de ventas por pais:\n";
    for (auto& [pais, ciudades] : ventasPorCiudadYPais) {
        vector<pair<string, double>> ciudadVector(ciudades.begin(), ciudades.end());
        sort(ciudadVector.begin(), ciudadVector.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });
        cout << "Pais: " << pais << endl;
        for (int i = 0; i < min(5, (int)ciudadVector.size()); ++i) {
            cout << "  " << ciudadVector[i].first << ": $" << ciudadVector[i].second << endl;
        }
    }
}

void mostrarMontoPorProductoYPais() {
    cout << "\nMonto total vendido por producto por pais: " << endl;
    for (auto& [pais, productos] : ventasPorProductoYPais) {
        cout << "Pais: " << pais << endl;
        for (auto& [producto, monto] : productos) {
            cout << "  " << producto << ": $" << monto << endl;
        }
    }
}

void mostrarPromedioPorCategoriaYPais() {
    cout << "\nPromedio de ventas por categoria en cada pais:\n";
    for (auto& [pais, categorias] : ventasPorCategoriaYPais) {
        cout << "Pais: " << pais << endl;
        for (auto& [categoria, data] : categorias) {
            double promedio = data.first / data.second;
            cout << "  " << categoria << ": $" << promedio << endl;
        }
    }
}

void mostrarEnvioMasUsadoPorPais() {
    cout << "\nMedio de envio mas utilizado por pais:\n";
    for (auto& [pais, medios] : envioPorPais) {
        string medioMax;
        int maxCount = 0;
        for (auto& [medio, count] : medios) {
            if (count > maxCount) {
                medioMax = medio;
                maxCount = count;
            }
            contadorIfs++;
        }
        cout << "Pais: " << pais << " - Medio: " << medioMax << " (" << maxCount << " veces)" << endl;
    }
}

void mostrarEnvioMasUsadoPorCategoria() {
    cout << "\nMedio de envio mas utilizado por categoria:\n";
    for (auto& [categoria, medios] : envioPorCategoria) {
        string medioMax;
        int maxCount = 0;
        for (auto& [medio, count] : medios) {
            if (count > maxCount) {
                medioMax = medio;
                maxCount = count;
            }
            contadorIfs++;
        }
        cout << "Categoria: " << categoria << " - Medio: " << medioMax << " (" << maxCount << " veces)" << endl;
    }
}

void mostrarDiaMayorVenta() {
    cout << "\nDia con mayor cantidad de ventas (por monto):\n";
    string diaMax;
    double maxMonto = 0;
    for (auto& [fecha, monto] : ventasPorDia) {
        if (monto > maxMonto) {
            diaMax = fecha;
            maxMonto = monto;
        }
        contadorIfs++;
    }
    cout << diaMax << " con $" << maxMonto << endl;
}

void mostrarEstadoEnvioFrecuentePorPais() {
    cout << "\nEstado de envio mas frecuente por pais:\n";
    for (auto& [pais, estados] : estadoEnvioPorPais) {
        string estadoMax;
        int maxCount = 0;
        for (auto& [estado, count] : estados) {
            if (count > maxCount) {
                estadoMax = estado;
                maxCount = count;
            }
            contadorIfs++;
        }
        cout << "Pais: " << pais << " - Estado: " << estadoMax << " (" << maxCount << " veces)" << endl;
    }
}

void mostrarProductoMasYMenosVendido() {
    cout << "\nProducto mas y menos vendido (en unidades):\n";
    string maxProd, minProd;
    int maxCant = -1, minCant = INT_MAX;
    for (auto& [producto, cant] : unidadesVendidas) {
        if (cant > maxCant) {
            maxCant = cant;
            maxProd = producto;
        }
        contadorIfs++;
        if (cant < minCant) {
            minCant = cant;
            minProd = producto;
        }
        contadorIfs++;
    }
    cout << "Mas vendido: " << maxProd << " con " << maxCant << " unidades\n";
    cout << "Menos vendido: " << minProd << " con " << minCant << " unidades\n";
}

int main() {
    auto inicio = chrono::high_resolution_clock::now();

    procesarArchivo();
    mostrarTopCiudadesPorPais();
    mostrarMontoPorProductoYPais();
    mostrarPromedioPorCategoriaYPais();
    mostrarEnvioMasUsadoPorPais();
    mostrarEnvioMasUsadoPorCategoria();
    mostrarDiaMayorVenta();
    mostrarEstadoEnvioFrecuentePorPais();
    mostrarProductoMasYMenosVendido();

    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion = fin - inicio;

    cout << "\nTiempo de ejecucion: " << duracion.count() << " segundos\n";
    cout << "Contador de ifs explicitos: " << contadorIfs << endl;
 
    return 0;
}