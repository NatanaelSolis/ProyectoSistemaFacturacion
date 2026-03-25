#pragma once
#include <vector>
#include "../models/Producto.h"

class ProductoRepository
{
public:
    std::vector<Producto> obtenerTodos();
    bool insertarProducto(const Producto& producto);
    bool eliminarProductoPorCodigo(int codigo);
};
