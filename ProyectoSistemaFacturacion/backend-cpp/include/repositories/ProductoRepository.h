#pragma once

#include <vector>
#include "../models/Producto.h"
#include "../clients/ApiProductoClient.h"

class ProductoRepository
{
private:
    ApiProductoClient apiProductoClient;

public:
    ProductoRepository();

    std::vector<Producto> obtenerTodos();
    bool insertarProducto(const Producto& producto);
    bool eliminarProductoPorCodigo(int codigo);
};