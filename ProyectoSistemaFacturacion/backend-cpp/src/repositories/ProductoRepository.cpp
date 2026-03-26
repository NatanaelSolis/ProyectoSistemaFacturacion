#include "../../include/repositories/ProductoRepository.h"

ProductoRepository::ProductoRepository()
    : apiProductoClient(L"localhost", 5251, false)
{
}

std::vector<Producto> ProductoRepository::obtenerTodos()
{
    return apiProductoClient.obtenerProductosIniciales();
}

bool ProductoRepository::insertarProducto(const Producto& producto)
{
    return apiProductoClient.insertarProducto(producto);
}

bool ProductoRepository::eliminarProductoPorCodigo(int codigo)
{
    return apiProductoClient.eliminarProductoPorCodigo(codigo);
}