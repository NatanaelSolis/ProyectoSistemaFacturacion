#include "../../include/services/InventarioService.h"

// Registra un producto en el inventario
void InventarioService::registrarProducto(const Producto& producto)
{
    arbolProductos.insertarProducto(producto);
}

// Busca un producto por código
Producto* InventarioService::buscarProductoPorCodigo(int codigo)
{
    return arbolProductos.buscarProductoPorCodigo(codigo);
}

// Elimina un producto por código
void InventarioService::eliminarProductoPorCodigo(int codigo)
{
    arbolProductos.eliminarProductoPorCodigo(codigo);
}

// Muestra todos los productos en orden
void InventarioService::mostrarInventario()
{
    arbolProductos.mostrarEnOrden();
}