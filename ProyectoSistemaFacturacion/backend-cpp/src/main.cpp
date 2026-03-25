#include <iostream>
#include <string>
#include "../include/services/InventarioService.h"

using namespace std;

void mostrarMenu()
{
    cout << endl;
    cout << "========= MENU INVENTARIO =========" << endl;
    cout << "1. Registrar producto" << endl;
    cout << "2. Buscar producto por codigo" << endl;
    cout << "3. Eliminar producto por codigo" << endl;
    cout << "4. Mostrar inventario" << endl;
    cout << "5. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main()
{
    InventarioService inventario;
    int opcion = 0;

    do
    {
        mostrarMenu();
        cin >> opcion;

        switch (opcion)
        {
        case 1:
        {
            int codigo;
            string nombre;
            string categoria;
            string marca;
            double precio;
            int stock;
            string estado;

            cout << "Codigo: ";
            cin >> codigo;

            cin.ignore();

            cout << "Nombre: ";
            getline(cin, nombre);

            cout << "Categoria: ";
            getline(cin, categoria);

            cout << "Marca: ";
            getline(cin, marca);

            cout << "Precio: ";
            cin >> precio;

            cout << "Stock: ";
            cin >> stock;

            cin.ignore();

            cout << "Estado: ";
            getline(cin, estado);

            Producto nuevoProducto(codigo, nombre, categoria, marca, precio, stock, estado);
            inventario.registrarProducto(nuevoProducto);

            cout << "Producto registrado correctamente." << endl;
            break;
        }

        case 2:
        {
            int codigo;
            cout << "Ingrese el codigo a buscar: ";
            cin >> codigo;

            Producto* encontrado = inventario.buscarProductoPorCodigo(codigo);

            if (encontrado != nullptr)
            {
                cout << endl << "Producto encontrado:" << endl;
                encontrado->mostrar();
            }
            else
            {
                cout << "Producto no encontrado." << endl;
            }
            break;
        }

        case 3:
        {
            int codigo;
            cout << "Ingrese el codigo a eliminar: ";
            cin >> codigo;

            inventario.eliminarProductoPorCodigo(codigo);
            cout << "Operacion de eliminacion ejecutada." << endl;
            break;
        }

        case 4:
        {
            cout << endl << "=== INVENTARIO ACTUAL ===" << endl;
            inventario.mostrarInventario();
            break;
        }

        case 5:
        {
            cout << "Saliendo del sistema..." << endl;
            break;
        }

        default:
        {
            cout << "Opcion invalida." << endl;
            break;
        }
        }

    } while (opcion != 5);

    return 0;
}