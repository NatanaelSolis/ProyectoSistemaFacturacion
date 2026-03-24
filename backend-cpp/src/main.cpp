#include <iostream>
#include "../include/structures/ArbolAVL.h"

using namespace std;

int main()
{
    ArbolAVL arbol;

    arbol.insertarProducto(Producto(1001, "Sauvage", "Perfume", "Dior", 52990, 8, "Activo"));
    arbol.insertarProducto(Producto(1005, "Light Blue", "Perfume", "Dolce & Gabbana", 48990, 5, "Activo"));
    arbol.insertarProducto(Producto(1003, "212 VIP", "Perfume", "Carolina Herrera", 45990, 10, "Activo"));

    cout << "=== INVENTARIO INICIAL ===" << endl;
    arbol.mostrarEnOrden();

    cout << endl << "=== BUSCAR 1003 ===" << endl;
    Producto* encontrado = arbol.buscarProductoPorCodigo(1003);

    if (encontrado != nullptr)
    {
        encontrado->mostrar();
    }
    else
    {
        cout << "Producto no encontrado" << endl;
    }

    cout << endl << "=== ELIMINAR 1005 ===" << endl;
    arbol.eliminarProductoPorCodigo(1005);

    cout << endl << "=== INVENTARIO FINAL ===" << endl;
    arbol.mostrarEnOrden();

    return 0;
}