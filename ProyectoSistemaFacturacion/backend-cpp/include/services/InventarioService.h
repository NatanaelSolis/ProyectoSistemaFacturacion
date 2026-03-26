#pragma once

#include "../structures/ArbolAVL.h"
#include "../repositories/ProductoRepository.h"

class InventarioService
{
private:
    ArbolAVL arbolProductos;
    ProductoRepository productoRepository;

public:
    InventarioService();

    int cargarProductosIniciales();
    bool registrarProducto(const Producto& producto);
    Producto* buscarProductoPorCodigo(int codigo);
    bool eliminarProductoPorCodigo(int codigo);
    void mostrarInventario();
};