#pragma once
#include "../structures/ArbolAVL.h"

class InventarioService
{
private:
    ArbolAVL arbolProductos;

public:
    void registrarProducto(const Producto& producto);
    Producto* buscarProductoPorCodigo(int codigo);
    void eliminarProductoPorCodigo(int codigo);
    void mostrarInventario();
};