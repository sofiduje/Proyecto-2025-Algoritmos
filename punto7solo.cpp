//PUNTO 7
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <limits>

using namespace std;

// Alias de estructuras
using ProductoData = pair<int, double>;
using HashProductos = unordered_map<string, ProductoData>;
using HashPaises = unordered_map<string, HashProductos>;

// === Estructuras ===
struct Venta {
    string idVenta, fecha, pais, ciudad, cliente, producto, categoria, medioEnvio, estadoEnvio;
    int cantidad;
    double precioUnitario, montoTotal;
};

struct NodoAVL {
    string fecha;  // AAAA-MM-DD
    vector<Venta> ventas;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;
};

// === Variables globales ===
unordered_map<string, unordered_map<string, vector<Venta>>> ventasPorUbicacion;
unordered_map<string, NodoAVL*> ventasPorPais; 
unordered_map<string, double> ventas_por_pais; //C
unordered_map<string, unordered_map<string, int>> cantidad_producto_por_pais; //C
unordered_map<string, unordered_map<string, int>> envios_por_pais; //C
unordered_map<string, unordered_map<string, pair<int, double>>> productoPaisDatos; //D
HashPaises datosPromedios; //E

// === Declaración de funciones utilizadas en cargarCSV ===

string convertirFechaOrdenable(const string& fechaOriginal);
NodoAVL* insertarAVL(NodoAVL* nodo, const string& fecha, const Venta& venta);

// === Funciones AVL ===

int altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int balance(NodoAVL* nodo) {
    return nodo ? altura(nodo->izq) - altura(nodo->der) : 0;
}

void actualizarAltura(NodoAVL* nodo) {
    if (nodo)
        nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
}

NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

NodoAVL* insertarAVL(NodoAVL* nodo, const string& fecha, const Venta& venta) {
    if (!nodo) return new NodoAVL{fecha, {venta}, nullptr, nullptr, 1};

    if (fecha < nodo->fecha)
        nodo->izq = insertarAVL(nodo->izq, fecha, venta);
    else if (fecha > nodo->fecha)
        nodo->der = insertarAVL(nodo->der, fecha, venta);
    else {
        nodo->ventas.push_back(venta);
        return nodo;
    }

    actualizarAltura(nodo);
    int balanceFactor = balance(nodo);

    if (balanceFactor > 1 && fecha < nodo->izq->fecha)
        return rotarDerecha(nodo);
    if (balanceFactor < -1 && fecha > nodo->der->fecha)
        return rotarIzquierda(nodo);
    if (balanceFactor > 1 && fecha > nodo->izq->fecha) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (balanceFactor < -1 && fecha < nodo->der->fecha) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// === Funciones auxiliares ===

// Convierte fecha de DD-MM-AAAA a AAAA-MM-DD para ordenamiento lexicográfico
string convertirFechaOrdenable(const string& fechaOriginal) {
    string dia = fechaOriginal.substr(0, 2);
    string mes = fechaOriginal.substr(3, 2);
    string anio = fechaOriginal.substr(6, 4);
    return anio + "-" + mes + "-" + dia;
}

// Valida formato de fecha DD-MM-AAAA
bool fechaValida(const string& fecha) {
    if (fecha.length() != 10) return false;
    if (fecha[2] != '-' || fecha[5] != '-') return false;

    for (int i = 0; i < (int)fecha.length(); ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(fecha[i])) return false;
    }

    int dia = stoi(fecha.substr(0, 2));
    int mes = stoi(fecha.substr(3, 2));
    int anio = stoi(fecha.substr(6, 4));

    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || anio < 1900 || anio > 2100)
        return false;

    return true;
}

// === Función cargarCSV ===

void cargarCSV(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // Saltear encabezado

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Venta v;

        getline(ss, v.idVenta, ',');
        getline(ss, campo, ','); v.fecha = convertirFechaOrdenable(campo);
        getline(ss, v.pais, ',');
        getline(ss, v.ciudad, ',');
        getline(ss, v.cliente, ',');
        getline(ss, v.producto, ',');
        getline(ss, v.categoria, ',');
        getline(ss, campo, ','); v.cantidad = stoi(campo);
        getline(ss, campo, ','); v.precioUnitario = stod(campo);
        getline(ss, campo, ','); v.montoTotal = stod(campo);
        getline(ss, v.medioEnvio, ',');
        getline(ss, v.estadoEnvio, ',');

        // Guardar en mapa por ubicación
        ventasPorUbicacion[v.pais][v.ciudad].push_back(v);

        // Insertar en AVL por país
        ventasPorPais[v.pais] = insertarAVL(ventasPorPais[v.pais], v.fecha, v);

        ventas_por_pais[v.pais] += v.montoTotal;
        cantidad_producto_por_pais[v.pais][v.producto] += v.cantidad;
        envios_por_pais[v.pais][v.medioEnvio]++;

        productoPaisDatos[v.producto][v.pais].first += v.cantidad;
        productoPaisDatos[v.producto][v.pais].second += v.montoTotal;

        datosPromedios[v.pais][v.producto].first += v.cantidad;
        datosPromedios[v.pais][v.producto].second += v.montoTotal;
    }

    archivo.close();
}

// === Función para mostrar ventas en una ciudad específica ===
void mostrarVentasDeCiudad() {
    string ciudadBuscada;
    bool encontrada = false;

    while (!encontrada) {
        cout << "Ingrese el nombre de la ciudad: ";
        getline(cin >> ws, ciudadBuscada);

        for (const auto& parPais : ventasPorUbicacion) {
            const string& pais = parPais.first;
            const auto& mapaCiudades = parPais.second;

            auto itCiudad = mapaCiudades.find(ciudadBuscada);
            if (itCiudad != mapaCiudades.end()) {
                vector<Venta> ventas = itCiudad->second;
                sort(ventas.begin(), ventas.end(), [](const Venta& a, const Venta& b) {
                    return a.producto < b.producto;
                });

                cout << "\nVentas en " << ciudadBuscada << " (" << pais << "):\n";
                for (const Venta& v : ventas) {
                    cout << "ID: " << v.idVenta
                         << " | Producto: " << v.producto
                         << " | Categoria: " << v.categoria
                         << " | Cliente: " << v.cliente
                         << " | Cantidad: " << v.cantidad
                         << " | Monto: $" << v.montoTotal
                         << " | Fecha: " << v.fecha
                         << " | Medio: " << v.medioEnvio
                         << " | Estado: " << v.estadoEnvio << '\n';
                }

                encontrada = true;
                break;
            }
        }

        if (!encontrada) {
            cout << "Ciudad invalida. Intente nuevamente.\n";
        }
    }
}

// === Función para listar ventas en rango de fechas por país ===
void listarVentasPorRango(NodoAVL* nodo, const string& desde, const string& hasta) {
    if (!nodo) return;
    if (desde < nodo->fecha)
        listarVentasPorRango(nodo->izq, desde, hasta);
    if (desde <= nodo->fecha && nodo->fecha <= hasta) {
        for (const Venta& v : nodo->ventas) {
            cout << "ID: " << v.idVenta 
            << " | Fecha: " << v.fecha
            << " | Producto: " << v.producto 
            << " | Cliente: " << v.cliente
            << " | Monto: $" << v.montoTotal << endl;
        }
    }
    if (hasta > nodo->fecha)
        listarVentasPorRango(nodo->der, desde, hasta);
}

void mostrarVentasPorRangos() {
    string paisUsuario, fechaDesde, fechaHasta;

    cout << "\nIngrese el pais: ";
    getline(cin, paisUsuario);

    while (ventasPorPais.find(paisUsuario) == ventasPorPais.end()) {
        cout << "No hay registros para ese pais. Ingrese nuevamente: ";
        getline(cin, paisUsuario);
    }

    cout << "Ingrese la fecha desde (DD-MM-AAAA): ";
    getline(cin, fechaDesde);
    while (!fechaValida(fechaDesde)) {
        cout << "Fecha invalida. Ingrese nuevamente (DD-MM-AAAA): ";
        getline(cin, fechaDesde);
    }

    cout << "Ingrese la fecha hasta (DD-MM-AAAA): ";
    getline(cin, fechaHasta);
    while (!fechaValida(fechaHasta)) {
        cout << "Fecha invalida. Ingrese nuevamente (DD-MM-AAAA): ";
        getline(cin, fechaHasta);
    }

    string desdeOrdenable = convertirFechaOrdenable(fechaDesde);
    string hastaOrdenable = convertirFechaOrdenable(fechaHasta);

    cout << "\nVentas en " << paisUsuario << " del " << fechaDesde << " al " << fechaHasta << ":\n";
    listarVentasPorRango(ventasPorPais[paisUsuario], desdeOrdenable, hastaOrdenable);
}

// Mostrar comparación de monto total entre dos países
void compararMontoTotalVentas(string pais1, string pais2) {
    if (ventas_por_pais.count(pais1) && ventas_por_pais.count(pais2)) {
        cout << pais1 << ": $" << ventas_por_pais[pais1] << endl;
        cout << pais2 << ": $" << ventas_por_pais[pais2] << endl;

    } else {
        cout << "Uno o ambos paises no se encuentran en la base de datos." << endl;
    }
}
// Obtener producto más vendido en unidades por país
string productoMasVendido(string pais) {
    string productoMax = "";
    int maxCantidad = 0;

    for (auto& par : cantidad_producto_por_pais[pais]) {
        if (par.second > maxCantidad) {
            maxCantidad = par.second;
            productoMax = par.first;
        }
    }

    return productoMax;
}
// Obtener medio de envío más usado por país
string medioEnvioMasUsado(string pais) {
    string medioMax = "";
    int maxCantidad = 0;

    for (auto& par : envios_por_pais[pais]) {
        if (par.second > maxCantidad) {
            maxCantidad = par.second;
            medioMax = par.first;
        }
    }

    return medioMax;
}
//Verificar si los países existen en la base de datos
bool paisExiste(const string& pais) {
    return ventas_por_pais.find(pais) != ventas_por_pais.end();
}
void comparacionEntreDosPaises(){
    string pais1, pais2;

    cout << "=== COMPARACION ENTRE DOS PAISES ===" << endl;

    do {
        cout << "Ingrese el primer pais: ";
        getline(cin, pais1);
        if (!paisExiste(pais1)) {
            cout << "Pais no encontrado. Intente nuevamente.\n";
        }
    } while (!paisExiste(pais1));

    do {
        cout << "Ingrese el segundo pais: ";
        getline(cin, pais2);
        if (!paisExiste(pais2)) {
            cout << "Pais no encontrado. Intente nuevamente.\n";
        }
    } while (!paisExiste(pais2));

    cout << "\n--- Monto total de ventas ---" << endl;
    compararMontoTotalVentas(pais1, pais2);

    cout << "\n--- Producto mas vendido ---" << endl;
    cout << pais1 << ": " << productoMasVendido(pais1) << endl;
    cout << pais2 << ": " << productoMasVendido(pais2) << endl;

    cout << "\n--- Medio de envio mas usado ---" << endl;
    cout << pais1 << ": " << medioEnvioMasUsado(pais1) << endl;
    cout << pais2 << ": " << medioEnvioMasUsado(pais2) << endl;

}

void compararProductosPorPais(const string& producto1, const string& producto2) {
    cout << "\nComparacion de productos: " << producto1 << " vs " << producto2 << "\n\n";

    unordered_map<string, pair<int, double>> datos1 = productoPaisDatos[producto1];
    unordered_map<string, pair<int, double>> datos2 = productoPaisDatos[producto2];

    // Unión de países presentes en ambos productos
    unordered_map<string, bool> paises;
    for (const auto& p : datos1) paises[p.first] = true;
    for (const auto& p : datos2) paises[p.first] = true;

    for (const auto& entry : paises) {
        string pais = entry.first;
        int cant1 = datos1.count(pais) ? datos1[pais].first : 0;
        double monto1 = datos1.count(pais) ? datos1[pais].second : 0.0;

        int cant2 = datos2.count(pais) ? datos2[pais].first : 0;
        double monto2 = datos2.count(pais) ? datos2[pais].second : 0.0;

        cout << "Pais: " << pais << "\n";
        cout << "  " << producto1 << " - Cantidad: " << cant1 << ", Monto: $" << monto1 << "\n";
        cout << "  " << producto2 << " - Cantidad: " << cant2 << ", Monto: $" << monto2 << "\n\n";
    }
}
void compararDosProductos(){
    string producto1, producto2;

    // Validar producto 1
    while (true) {
        cout << "Ingrese el primer producto a comparar: ";
        getline(cin, producto1);

        if (productoPaisDatos.count(producto1) > 0) break;

        cout << "Producto '" << producto1 << "' no encontrado.\n";
        cout << "Productos disponibles:\n";
        for (const auto& p : productoPaisDatos) {
            cout << " - " << p.first << "\n";
        }
    }

    // Validar producto 2
    while (true) {
        cout << "Ingrese el segundo producto a comparar: ";
        getline(cin, producto2);

        if (productoPaisDatos.count(producto2) > 0) break;

        cout << "Producto '" << producto2 << "' no encontrado.\n";
        cout << "Productos disponibles:\n";
        for (const auto& p : productoPaisDatos) {
            cout << " - " << p.first << "\n";
        }
    }

    compararProductosPorPais(producto1, producto2);

    return;
}

// e. Mostrar productos filtrados por promedio y umbral
void mostrarProductosFiltrados(
    const HashPaises& datos,
    const string& paisUsuario,
    float umbral,
    const string& condicion
) {
    function<bool(float, float)> comparar;
    comparar = (condicion == "menor") ?
        [](float a, float b) { return a < b; } :
        [](float a, float b) { return a > b; };

    auto it = datos.find(paisUsuario);
    if (it == datos.end()) {
        cout << "Pais no encontrado." << endl;
        return;
    }

    vector<pair<string, float>> productosFiltrados;

    for (const auto& [producto, info] : it->second) {
        int cantidad = info.first;
        float monto = info.second;

        if (cantidad > 0) {
            float promedio = monto / cantidad;
            if (comparar(promedio, umbral)) {
                productosFiltrados.push_back({producto, promedio});
            }
        }
    }

    if (productosFiltrados.empty()) {
        cout << "No se encontraron productos con promedio " << condicion
             << " al umbral de $" << umbral << " en " << paisUsuario << "." << endl;
    } else {
        cout << "Productos con promedio " << condicion << " al umbral de $" << umbral
             << " en " << paisUsuario << ":" << endl;
        for (const auto& [producto, promedio] : productosFiltrados) {
            cout << " - " << producto << " (Promedio: $" << promedio << ")" << endl;
        }
    }
}

void promedioVentasSegunUmbral(){
    string pais, condicion;
    float umbral;

    cout << "\n=== FILTRAR PRODUCTOS POR PROMEDIO Y PAIS ===\n";

    //Validar país
    while (true) {
        cout << "Ingrese el nombre del pais: ";
        getline(cin, pais);

        if (datosPromedios.find(pais) != datosPromedios.end()) break;
        cout << "Pais no encontrado. Intente nuevamente." << endl;
    }

    //Validar umbral
    string entrada;
    while (true) {
        cout << "Ingrese el umbral ($): ";
        getline(cin, entrada);

        try {
            umbral = stof(entrada);
            if (umbral < 0) throw invalid_argument("Negativo");
            break;
        } catch (...) {
            cout << "Ingrese un numero valido mayor o igual a cero." << endl;
        }
    }

    // Validar condición: mayor o menor
    while (true) {
        cout << "Desea ver productos con promedio mayor o menor al umbral? ";
        getline(cin, condicion);

        // convertir a minúsculas
        for (auto& c : condicion) c = tolower(c);

        if (condicion == "mayor" || condicion == "menor") break;

        cout << "Condicion invalida. Ingrese 'mayor' o 'menor'." << endl;
    }

    // Mostrar productos filtrados
    mostrarProductosFiltrados(datosPromedios, pais, umbral, condicion);
}

void menu7() {
    int opcion;
    do {
        cout << "\n=== MENU DE CONSULTAS ===\n";
        cout << "1. Mostrar ventas de una ciudad\n";
        cout << "2. Mostrar ventas por rango de fechas en un pais\n";
        cout << "3. Comparacion entre dos paises\n";
        cout << "4. Comparar dos productos\n";
        cout << "5. Promedio de ventas segun umbral\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        //Validando la entrada
        while (!(cin >> opcion) || opcion < 0 || opcion > 5) {
            cout << "Entrada invalida. Por favor, seleccione una opcion valida (0-5): ";
            cin.clear(); // Limpia el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta el resto de la línea
        }
        
        cin.ignore();  // Limpiar buffer

        switch (opcion) {
            case 1:
                mostrarVentasDeCiudad();
                break;
            case 2:
                mostrarVentasPorRangos();
                break;
            case 3:
                comparacionEntreDosPaises();
                break;
            case 4:
                compararDosProductos();  
                break;
            case 5:
                promedioVentasSegunUmbral(); 
                break;
            case 0:
                cout << "Saliendo del menu...\n";
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }

    } while (opcion != 0);
}

int main() {
    cargarCSV("ventas_sudamerica(1).csv");
    menu7();
    return 0;
}

