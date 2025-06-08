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
#include <limits>
#include <functional>

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


// Funciones para mostrar resultados
// === TOP 5 DE CIUDADES CON MAYOR MONTO DE VENTAS POR PAÍS ===
void mostrarTopCiudadesPorPais() {
    cout << "Top 5 ciudades con mayor monto de ventas por pais:\n";
    for (auto& [pais, ciudades] : ventasPorCiudadYPais) {
        vector<pair<string, double>> ciudadVector(ciudades.begin(), ciudades.end());
        sort(ciudadVector.begin(), ciudadVector.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });
        contadorIfs++; 
        cout << "Pais: " << pais << endl;
        for (int i = 0; i < min(5, (int)ciudadVector.size()); ++i) {
            cout << "  " << ciudadVector[i].first << ": $" << ciudadVector[i].second << endl;
        }
    }
}

// === MONTO TOTAL VENDIDO POR PRODUCTO, DISCRIMINADO POR PAÍS ===
void mostrarMontoPorProductoYPais() {
    cout << "\nMonto total vendido por producto por pais: " << endl;
    for (auto& [pais, productos] : ventasPorProductoYPais) {
        cout << "Pais: " << pais << endl;
        for (auto& [producto, monto] : productos) {
            cout << "  " << producto << ": $" << monto << endl;
        }
    }
}

//=== PROMEDIO DE VENTAS POR CATEGORÍA EN CADA PAÍS ===
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

//=== MEDIO DE ENVÍO MÁS UTILIZADO POR PAÍS ===
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

//=== MEDIO DE ENVÍO MÁS UTILIZADO POR CATEGORÍA ===
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

// === DÍA CON MAYOR CANTIDAD DE VENTAS EN TODA LA BASE DE DATOS
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

//=== ESTADO DE ENVÍO MÁS FRECUENTE POR PAÍS ===
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

//=== PRODUCTO MAS Y MENOS VENDIDO EN CANTIDAD TOTAL ===
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
    cout << "Menos vendido: " << minProd << " con " << minCant << " unidades" << endl;
}

//punto 7 definiciones _____________________________________________________________________________________________
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

//PROCESAR ARCHIVO _______________________________________________________________________________________________
void procesarArchivo() {
    ifstream archivo("ventas_sudamerica(1).csv");
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo ventas_sudamerica(1).csv\n";
        return;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado

    int nroLinea = 1;
    while (getline(archivo, linea)) {
        nroLinea++;
        stringstream ss(linea);
        string campo;
        Venta v;

        try {
            // Lectura de campos
            getline(ss, v.idVenta, ',');
            getline(ss, campo, ','); v.fecha = convertirFechaOrdenable(campo);
            getline(ss, v.pais, ',');
            getline(ss, v.ciudad, ',');
            getline(ss, v.cliente, ',');
            getline(ss, v.producto, ',');
            getline(ss, v.categoria, ',');
            getline(ss, campo, ','); v.cantidad = stoi(campo);
            getline(ss, campo, ','); v.precioUnitario = stod(campo);
            getline(ss, campo, ',');
            
            if (campo.empty()) {
                cerr << "Linea " << nroLinea << ": monto vacio, se omite registro.\n";
                continue;
            }
            v.montoTotal = stod(campo);

            getline(ss, v.medioEnvio, ',');
            getline(ss, v.estadoEnvio, ',');

            // Estructuras de cargarCSV()
            ventasPorUbicacion[v.pais][v.ciudad].push_back(v);
            ventasPorPais[v.pais] = insertarAVL(ventasPorPais[v.pais], v.fecha, v);
            ventas_por_pais[v.pais] += v.montoTotal;
            cantidad_producto_por_pais[v.pais][v.producto] += v.cantidad;
            envios_por_pais[v.pais][v.medioEnvio]++;
            productoPaisDatos[v.producto][v.pais].first += v.cantidad;
            productoPaisDatos[v.producto][v.pais].second += v.montoTotal;
            datosPromedios[v.pais][v.producto].first += v.cantidad;
            datosPromedios[v.pais][v.producto].second += v.montoTotal;

            // Estructuras de procesarArchivo()
            ventasPorCiudadYPais[v.pais][v.ciudad] += v.montoTotal;
            ventasPorProductoYPais[v.pais][v.producto] += v.montoTotal;
            ventasPorCategoriaYPais[v.pais][v.categoria].first += v.montoTotal;
            ventasPorCategoriaYPais[v.pais][v.categoria].second += 1;
            envioPorPais[v.pais][v.medioEnvio]++;
            envioPorCategoria[v.categoria][v.medioEnvio]++;
            ventasPorDia[v.fecha] += v.montoTotal;
            estadoEnvioPorPais[v.pais][v.estadoEnvio]++;
            unidadesVendidas[v.producto] += v.cantidad;

        } catch (const std::invalid_argument& e) {
            cerr << "Linea " << nroLinea << ": dato no convertible a numero. monto o cantidad invalidos.\n";
            continue;
        } catch (const std::out_of_range& e) {
            cerr << "Linea " << nroLinea << ": valor fuera de rango para conversion numerica.\n";
            continue;
        }
    }

    archivo.close();
}

//PUNTO 6
HashMapList<string, string> mapaVentas(100, [](string clave) {
    unsigned int hash = 0;
    for (char c : clave) hash += c;
    return hash;
});
vector<string> idsVentas;

void reprocesarTodo() {
    // Limpiar estructuras globales
    ventasPorCiudadYPais.clear();
    ventasPorProductoYPais.clear();
    ventasPorCategoriaYPais.clear();
    envioPorPais.clear();
    envioPorCategoria.clear();
    ventasPorDia.clear();
    estadoEnvioPorPais.clear();
    unidadesVendidas.clear();
    // Reprocesar CSV
    procesarArchivo();
}

//Para validar cada vez que agregamos una venta
// Validar si un string es un número entero
bool esEntero(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Validar si un string es un número flotante (punto decimal)
bool esFlotante(const string& s) {
    if (s.empty()) return false;
    bool punto = false;
    for (char c : s) {
        if (c == '.') {
            if (punto) return false;
            punto = true;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
// Validar que una cadena contenga solo letras
bool esPalabra(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}
// Validar fecha en formato dd-mm-aaaa
bool esFechaValida(const string& fecha) {
    if (fecha.size() != 10) return false;
    if (fecha[2] != '/' || fecha[5] != '/') return false;

    string diaStr = fecha.substr(0, 2);
    string mesStr = fecha.substr(3, 2);
    string anioStr = fecha.substr(6, 4);

    if (!esEntero(diaStr) || !esEntero(mesStr) || !esEntero(anioStr)) return false;

    int dia = stoi(diaStr);
    int mes = stoi(mesStr);
    int anio = stoi(anioStr);

    if (mes < 1 || mes > 12) return false;

    int diasPorMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    if (dia < 1 || dia > diasPorMes[mes - 1]) return false;

    return true;
}

// === AGREGAR VENTA ===
void agregarVenta() {
    string idVenta, fecha, pais, ciudad, cliente, producto, categoria, medioEnvio, estadoEnvio;
    string cantidadStr, precioUnitarioStr, montoTotalStr;
    int cantidad;
    float precioUnitario, montoTotal;

    // Validar ID (entero)
    while (true) {
        cout << "ID de la venta (entero): ";
        cin >> idVenta;
        if (esEntero(idVenta)) break;
        cout << "ID invalido. Debe ser un numero entero.\n";
    }
    cantidad = stoi(idVenta);

    // Validar fecha
    while (true) {
        cout << "Fecha (dd/mm/aaaa): ";
        cin >> fecha;
        if (esFechaValida(fecha)) break;
        cout << "Fecha invalida. Debe tener formato dd/mm/aaaa y ser valida.\n";
    }

    // Validar pais (solo letras y espacios)
    while (true) {
        cout << "Pais: ";
        cin.ignore();
        getline(cin, pais);
        if (esPalabra(pais)) break;
        cout << "Pais invalido. Solo se permiten letras y espacios.\n";
    }

    // Validar ciudad (solo letras y espacios)
    while (true) {
        cout << "Ciudad: ";
        getline(cin, ciudad);
        if (esPalabra(ciudad)) break;
        cout << "Ciudad invalida. Solo se permiten letras y espacios.\n";
    }

    // Validar cliente (solo letras y espacios)
    while (true) {
        cout << "Cliente: ";
        getline(cin, cliente);
        if (esPalabra(cliente)) break;
        cout << "Cliente invalido. Solo se permiten letras y espacios.\n";
    }

    // Validar producto (solo letras y espacios)
    while (true) {
        cout << "Producto: ";
        getline(cin, producto);
        if (esPalabra(producto)) break;
        cout << "Producto invalido. Solo se permiten letras y espacios.\n";
    }

    // Validar categoria (solo letras y espacios)
    while (true) {
        cout << "Categoria: ";
        getline(cin, categoria);
        if (esPalabra(categoria)) break;
        cout << "Categoria invalida. Solo se permiten letras y espacios.\n";
    }

    // Validar cantidad (entero)
    while (true) {
        cout << "Cantidad (entero): ";
        cin >> cantidadStr;
        if (esEntero(cantidadStr)) break;
        cout << "Cantidad invalida. Debe ser un numero entero.\n";
    }
    cantidad = stoi(cantidadStr);

    // Validar precioUnitario (flotante)
    while (true) {
        cout << "Precio unitario (numero decimal con punto): ";
        cin >> precioUnitarioStr;
        if (esFlotante(precioUnitarioStr)) break;
        cout << "Precio unitario invalido. Debe ser un numero decimal.\n";
    }
    precioUnitario = stof(precioUnitarioStr);

    // Validar montoTotal (flotante)
    while (true) {
        cout << "Monto total (numero decimal con punto): ";
        cin >> montoTotalStr;
        if (esFlotante(montoTotalStr)) break;
        cout << "Monto total invalido. Debe ser un numero decimal.\n";
    }
    montoTotal = stof(montoTotalStr);

    // Validar medioEnvio (solo letras y espacios)
    while (true) {
        cout << "Medio de envio: ";
        cin.ignore();
        getline(cin, medioEnvio);
        if (esPalabra(medioEnvio)) break;
        cout << "Medio de envio invalido. Solo se permiten letras y espacios.\n";
    }

    // Validar estadoEnvio (solo letras y espacios)
    while (true) {
        cout << "Estado de envio: ";
        getline(cin, estadoEnvio);
        if (esPalabra(estadoEnvio)) break;
        cout << "Estado de envio invalido. Solo se permiten letras y espacios.\n";
    }

    // Crear string para guardar
    stringstream nuevaVenta;
    nuevaVenta << idVenta << "," << fecha << ","
               << pais << "," << ciudad << "," << cliente << ","
               << producto << "," << categoria << "," << cantidad
               << "," << precioUnitario << "," << montoTotal << ","
               << medioEnvio << "," << estadoEnvio;

    // Guardar en la estructura y archivo
    mapaVentas.put(idVenta, nuevaVenta.str());

    ofstream archivo("ventas_sudamerica(1).csv", ios::app);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo para agregar la venta.\n";
        return;
    }
    archivo << nuevaVenta.str() << endl;
    archivo.close();

    reprocesarTodo();
    cout << "Venta agregada con exito." << endl;
}

//=== ELIMINAR VENTA ===
void cargarVentasDesdeCSV() {
    ifstream archivo("ventas_sudamerica(1).csv");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de ventas.\n";
        return;
    }

    string linea;
    getline(archivo, linea); // Saltear encabezado

    mapaVentas.clear();
    idsVentas.clear();

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campos[12];
        for (int i = 0; i < 12; ++i) {
            getline(ss, campos[i], ',');
        }
        string id = campos[0];
        mapaVentas.put(id, linea);
        idsVentas.push_back(id);
    }

    archivo.close();
}

void eliminarVenta() {
    cargarVentasDesdeCSV(); 
    cin.ignore();

    // Pedir opción mientras no sea válida
    int opcion;
string entrada;

while (true) {
    cout << "Eliminar venta por:\n";
    cout << "1. Pais\n";
    cout << "2. Ciudad\n";
    cout << "Ingrese opcion (1 o 2): ";
    
    getline(cin, entrada); // leer como string

    stringstream ss(entrada);
    if (ss >> opcion && (opcion == 1 || opcion == 2)) {
        break;
    }

    cout << "Entrada invalida. Intente nuevamente.\n";
}

    string filtro;
    vector<pair<string, string>> coincidencias;

    // Pedir filtro hasta encontrar coincidencias
    while (true) {
        cout << "Ingrese el " << (opcion == 1 ? "pais: " : "ciudad: ");
        getline(cin, filtro);

        coincidencias.clear();
        for (const string& id : idsVentas) {
            string linea = mapaVentas.get(id); 
            stringstream ss(linea);
            string campos[12];
            for (int j = 0; j < 12; ++j) {
                getline(ss, campos[j], ',');
            }

            string pais = campos[2];
            string ciudad = campos[3];

            if ((opcion == 1 && pais == filtro) || (opcion == 2 && ciudad == filtro)) {
                coincidencias.push_back({id, linea});
            }
        }

        if (coincidencias.empty()) {
            cout << "No se encontraron ventas para el filtro ingresado. Intente de nuevo.\n";
        } else {
            break;
        }
    }

    // Mostrar coincidencias
    cout << "\nVentas encontradas:\n";
    for (auto& venta : coincidencias) {
        cout << "ID: " << venta.first << " | " << venta.second << endl;
    }

    string idEliminar;

    // Pedir ID para eliminar hasta que sea válido
    while (true) {
        cout << "\nIngrese el ID de la venta que desea eliminar: ";
        getline(cin, idEliminar);

        auto itCoincidencia = find_if(coincidencias.begin(), coincidencias.end(),
            [&](const pair<string,string>& v){ return v.first == idEliminar; });

        if (itCoincidencia == coincidencias.end()) {
            cout << "El ID ingresado no coincide con las ventas listadas. Intente de nuevo.\n";
        } else {
            break;
        }
    }

    // Eliminar venta
    auto it = find(idsVentas.begin(), idsVentas.end(), idEliminar);
    if (it == idsVentas.end()) {
        cout << "Error inesperado: ID no encontrado en la lista.\n";
        return;
    }

    mapaVentas.remover(idEliminar);
    idsVentas.erase(it);

    // Reescribir archivo
    ofstream archivo("ventas_sudamerica(1).csv");
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para guardar los cambios.\n";
        return;
    }

    archivo << "ID_Venta,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio_Unitario,Monto_Total,Medio_Envio,Estado_Envio\n";
    for (const string& id : idsVentas) {
        archivo << mapaVentas.get(id) << endl;
    }
    archivo.close();

    reprocesarTodo();

    cout << "Venta eliminada correctamente.\n";
}

void modificarVenta() {
    cargarVentasDesdeCSV(); // carga mapaVentas y idsVentas

    string id;
    while (true) {
        cout << "Ingrese el ID de la venta a modificar: ";
        cin >> id;
        if (esEntero(id)) break;
        cout << "ID invalido. Debe ser un numero entero.\n";
    }

    string lineaOriginal = mapaVentas.get(id);
    if (lineaOriginal.empty()) {
        cout << "ID no encontrado.\n";
        return;
    }

    stringstream ss(lineaOriginal);
    string campos[12];
    for (int i = 0; i < 12; ++i) {
        getline(ss, campos[i], ',');
    }

    string etiquetas[12] = {
        "ID_Venta", "Fecha", "Pais", "Ciudad", "Cliente", "Producto", "Categoria",
        "Cantidad", "Precio_Unitario", "Monto_Total", "Medio_Envio", "Estado_Envio"
    };

    cout << "\nDatos actuales:\n";
    for (int i = 0; i < 12; ++i) {
        cout << etiquetas[i] << ": " << campos[i] << endl;
    }

    cout << "\nIngrese nuevos valores. Presione ENTER para mantener el valor actual:\n";
    cin.ignore(); // limpiar buffer

    for (int i = 1; i < 12; ++i) { // ID no se modifica
        string entrada;
        while (true) {
            cout << etiquetas[i] << " (" << campos[i] << "): ";
            getline(cin, entrada);

            if (entrada.empty()) break; // mantener valor actual

            bool valido = false;
            if (i == 1) valido = esFechaValida(entrada);               // Fecha
            else if (i >= 2 && i <= 6) valido = esPalabra(entrada);    // Pais, Ciudad, Cliente, Producto, Categoria
            else if (i == 7) valido = esEntero(entrada);               // Cantidad
            else if (i == 8 || i == 9) valido = esFlotante(entrada);   // Precio_Unitario, Monto_Total
            else if (i == 10 || i == 11) valido = esPalabra(entrada);  // Medio_Envio, Estado_Envio

            if (valido) {
                campos[i] = entrada;
                break;
            } else {
                cout << "Entrada invalida para " << etiquetas[i] << ". Intente de nuevo.\n";
            }
        }
    }

    // Reconstruir la línea
    stringstream nuevaLinea;
    for (int i = 0; i < 12; ++i) {
        nuevaLinea << campos[i];
        if (i != 11) nuevaLinea << ",";
    }

    mapaVentas.put(id, nuevaLinea.str());

    // Reescribir archivo
    ofstream archivo("ventas_sudamerica(1).csv");
    archivo << "ID_Venta,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio_Unitario,Monto_Total,Medio_Envio,Estado_Envio\n";
    for (const string& vid : idsVentas) {
        archivo << mapaVentas.get(vid) << endl;
    }

    archivo.close();
    reprocesarTodo();

    cout << "Venta modificada correctamente.\n";
}


// ________PUNTO 7__________________________________________________________________________________________
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


void menu() {
    int opcion;
    do {
        cout << "\nGestion de datos" << endl;
        cout << "1. Agregar venta" << endl;
        cout << "2. Eliminar venta" << endl;
        cout << "3. Modificar datos" << endl;
        cout << "4. Consultas dinamicas" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";

        //Validando la entrada
        while (!(cin >> opcion) || opcion < 0 || opcion > 4) {
            cout << "Entrada invalida. Ingrese un numero entre 0 y 4: ";
            cin.clear(); // Limpia flags de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta entrada basura
        }

        switch (opcion) {
        case 1:
            agregarVenta();
            break;
        case 2:
            eliminarVenta();
            break;
        case 3:
            modificarVenta();
            break;
        case 4:
            menu7(); 
            break;
        case 0:
            cout << "Saliendo del programa." << endl;
            break;
        }

    } while (opcion != 0);
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
 
    menu();

    return 0;

}