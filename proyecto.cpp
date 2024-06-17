#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip> // Para la función setprecision
#include <limits> // Para numeric_limits

using namespace std;

// Clase para generar un identificador único para cada producto
class GeneradorCodigo {
private:
    static int contador;
public:
    static int generarCodigo() {
        return ++contador;
    }
};

int GeneradorCodigo::contador = 0;

// Definición de la estructura de Producto
struct Producto {
    int codigoInterno; // Identificador único interno del producto
    string codigoExterno; // Código que el usuario asigna al producto
    string nombre;
    int cantidad;
    double precio;
};

// Mapa para almacenar el inventario de productos
map<string, Producto> inventario;

// Vector para almacenar el historial de ventas
vector<pair<string, double>> historialVentas;

// Funciones de validación de entrada
string obtenerEntradaString(const string& mensaje) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin >> ws, entrada); // ws descarta los espacios en blanco
        if (!entrada.empty()) break;
        cout << "Entrada inválida. Por favor, inténtelo nuevamente." << endl;
    }
    return entrada;
}

int obtenerEntradaInt(const string& mensaje) {
    int entrada;
    while (true) {
        cout << mensaje;
        cin >> entrada;
        if (cin.fail() || entrada < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Por favor, ingrese un número entero positivo." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return entrada;
}

double obtenerEntradaDouble(const string& mensaje) {
    double entrada;
    while (true) {
        cout << mensaje;
        cin >> entrada;
        if (cin.fail() || entrada < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Por favor, ingrese un número positivo." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return entrada;
}

// Función para registrar un nuevo producto en el inventario
void registroProducto() {
    string nombre = obtenerEntradaString("Ingrese el nombre del producto: ");
    string codigoExterno = obtenerEntradaString("Ingrese el código del producto: ");
    int cantidad = obtenerEntradaInt("Ingrese la cantidad del producto: ");
    double precio = obtenerEntradaDouble("Ingrese el precio del producto: ");

    // Generar el código interno único para el producto
    int codigoInterno = GeneradorCodigo::generarCodigo();

    // Agregar el producto al inventario
    Producto nuevoProducto = {codigoInterno, codigoExterno, nombre, cantidad, precio};
    inventario[codigoExterno] = nuevoProducto;

    cout << "Producto registrado exitosamente." << endl;
}

// Función para mostrar el inventario completo
void mostrarInventario() {
    if (inventario.empty()) {
        cout << "Aún no tienes productos en el inventario." << endl;
    } else {
        cout << "Inventario de productos:" << endl;
        for (const auto& par : inventario) {
            const Producto& producto = par.second;
            cout << "Código externo: " << producto.codigoExterno
                << ", Nombre: " << producto.nombre << ", Cantidad: " << producto.cantidad << ", Precio: $" << fixed << setprecision(2) << producto.precio << endl;
        }
    }
}

// Función para editar el inventario
void editarInventario() {
    string codigoExterno = obtenerEntradaString("Ingrese el código del producto que desea editar: ");

    if (inventario.find(codigoExterno) != inventario.end()) {
        Producto& producto = inventario[codigoExterno];
        char opcion;

        do {
            cout << "Seleccione el dato que desea cambiar:" << endl;
            cout << "  1. Nombre: " << producto.nombre << endl;
            cout << "  2. Código externo: " << producto.codigoExterno << endl;
            cout << "  3. Cantidad: " << producto.cantidad << endl;
            cout << "  4. Precio: " << fixed << setprecision(2) << producto.precio << endl;
            cout << "  5. Eliminar producto" << endl;
            cout << "  6. Finalizar edición" << endl;
            cout << "Opción: ";
            cin >> opcion;

            switch (opcion) {
                case '1':
                    producto.nombre = obtenerEntradaString("Nuevo nombre: ");
                    break;
                case '2': {
                    string nuevoCodigoExterno = obtenerEntradaString("Nuevo código externo: ");
                    inventario.erase(codigoExterno); // Eliminar la entrada antigua
                    producto.codigoExterno = nuevoCodigoExterno; // Actualizar el código externo
                    inventario[nuevoCodigoExterno] = producto; // Insertar con el nuevo código externo
                    codigoExterno = nuevoCodigoExterno; // Actualizar el identificador usado en el bucle
                    break;
                }
                case '3':
                    producto.cantidad = obtenerEntradaInt("Nueva cantidad: ");
                    break;
                case '4':
                    producto.precio = obtenerEntradaDouble("Nuevo precio: ");
                    break;
                case '5': {
                    // Confirmar eliminación
                    char confirmacion;
                    cout << "¿Está seguro de que desea eliminar este producto? (s/n): ";
                    cin >> confirmacion;
                    if (confirmacion == 's' || confirmacion == 'S') {
                        inventario.erase(codigoExterno);
                        cout << "Producto eliminado del inventario." << endl;
                        return; // Salir de la edición ya que el producto ha sido eliminado
                    }
                    break;
                }
                case '6':
                    cout << "Finalizando edición." << endl;
                    break;
                default:
                    cout << "Opción inválida. Por favor, seleccione nuevamente." << endl;
            }
        } while (opcion != '6');
    } else {
        cout << "El producto no se encuentra en el inventario." << endl;
    }
}

// Función para realizar una nueva venta
void nuevaVenta() {
    string codigoExterno = obtenerEntradaString("Ingrese el código del producto a vender: ");

    // Buscar el producto con el código externo actualizado
    for (auto& par : inventario) {
        if (par.second.codigoExterno == codigoExterno) {
            Producto& producto = par.second;
            int cantidad = obtenerEntradaInt("Ingrese la cantidad a vender: ");

            if (producto.cantidad >= cantidad) {
                // Realizar la venta
                double precioVenta = cantidad * producto.precio;
                producto.cantidad -= cantidad;
                historialVentas.push_back(make_pair(producto.nombre + " (Cantidad: " + to_string(cantidad) + ")", precioVenta));
                cout << "Venta realizada exitosamente. Precio total: $" << fixed << setprecision(2) << precioVenta << endl;
                return;
            } else {
                cout << "No hay suficiente cantidad en el inventario." << endl;
                return;
            }
        }
    }

    // Si no se encuentra el producto
    cout << "El producto no se encuentra en el inventario." << endl;
}

// Función para mostrar el registro de ventas
void mostrarRegistroVentas() {
    if (historialVentas.empty()) {
        cout << "Aún no se han realizado ventas." << endl;
    } else {
        cout << "Registro de ventas:" << endl;
        for (const auto& venta : historialVentas) {
            cout << "Producto: " << venta.first << ", Precio: $" << fixed << setprecision(2) << venta.second << endl;
        }
    }
}

// Función principal
int main() {
    char opcion;

    do {
        cout << "          MENU PRINCIPAL" << endl;
        cout << "Seleccione una opción:" << endl;
        cout << "  1. Inventario" << endl;
        cout << "  2. Ventas" << endl;
        cout << "  3. Salir" << endl;
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
            case '1': {
                char subopcion;
                do {
                    cout << "          INVENTARIO" << endl;
                    mostrarInventario();
                    cout << "Opciones:" << endl;
                    cout << "  1. Ingresar producto" << endl;
                    cout << "  2. Editar producto" << endl;
                    cout << "  3. Volver al menú principal" << endl;
                    cout << "Opción: ";
                    cin >> subopcion;
                    cout << endl;

                    switch (subopcion) {
                        case '1':
                            registroProducto();
                            break;
                        case '2':
                            editarInventario();
                            break;
                        case '3':
                            cout << "Volviendo al menú principal." << endl;
                            break;
                        default:
                            cout << "Opción inválida. Por favor, seleccione nuevamente." << endl;
                    }
                } while (subopcion != '3');
                break;
            }
            case '2': {
                char subopcionVenta;
                do {
                    cout << "          VENTAS" << endl;
                    cout << "Seleccione una opción:" << endl;
                    cout << "  1. Nueva venta" << endl;
                    cout << "  2. Registro de ventas" << endl;
                    cout << "  3. Volver al menú principal" << endl;
                    cout << "Opción: ";
                    cin >> subopcionVenta;
                    cout << endl;

                    switch (subopcionVenta) {
                        case '1':
                            nuevaVenta();
                            break;
                        case '2':
                            mostrarRegistroVentas();
                            break;
                        case '3':
                            cout << "Volviendo al menú principal." << endl;
                            break;
                        default:
                            cout << "Opción inválida. Por favor, seleccione nuevamente." << endl;
                    }
                } while (subopcionVenta != '3');
                break;
            }
            case '3':
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción inválida. Por favor, seleccione nuevamente." << endl;
        }
        cout << endl;
    } while (opcion != '3');

    return 0;
}