#include "../../include/services/InventarioService.h"

InventarioService::InventarioService()
{
}

int InventarioService::cargarProductosIniciales()
{
    std::vector<Producto> productos = productoRepository.obtenerTodos();

    for (const Producto& producto : productos)
    {
        arbolProductos.insertarProducto(producto);
    }

    return static_cast<int>(productos.size());
}

bool InventarioService::registrarProducto(const Producto& producto)
{
    Producto* existente = arbolProductos.buscarProductoPorCodigo(producto.getCodigo());

    if (existente != nullptr)
    {
        return false;
    }

    bool insertadoEnApi = productoRepository.insertarProducto(producto);

    if (!insertadoEnApi)
    {
        return false;
    }

    arbolProductos.insertarProducto(producto);
    return true;
}

Producto* InventarioService::buscarProductoPorCodigo(int codigo)
{
    return arbolProductos.buscarProductoPorCodigo(codigo);
}

bool InventarioService::eliminarProductoPorCodigo(int codigo)
{
    Producto* existente = arbolProductos.buscarProductoPorCodigo(codigo);

    if (existente == nullptr)
    {
        return false;
    }

    bool eliminadoEnApi = productoRepository.eliminarProductoPorCodigo(codigo);

    if (!eliminadoEnApi)
    {
        return false;
    }

    arbolProductos.eliminarProductoPorCodigo(codigo);
    return true;
}

void InventarioService::mostrarInventario()
{
    arbolProductos.mostrarEnOrden();
}