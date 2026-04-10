#pragma once

#include <vector>
#include "../structures/ArbolAVL.h"
#include "../repositories/ProductoRepository.h"
#include "../structures/TablaHashProductos.h"
#include "../structures/GrafoProductos.h"
#include "../structures/RelacionProducto.h"

class InventarioService
{
private:
    ArbolAVL arbolProductos;
    ProductoRepository productoRepository;
    TablaHashProductos hashPorNombre;
    TablaHashProductos hashPorMarca;
    TablaHashProductos hashPorCategoria;
    GrafoProductos grafoProductos;

    void indexarProductoEnHash(const Producto& producto);

public:
    InventarioService();

    int cargarProductosIniciales();
    bool registrarProducto(const Producto& producto);
    Producto* buscarProductoPorCodigo(int codigo);
    bool eliminarProductoPorCodigo(int codigo);
    void mostrarInventario();

    std::vector<Producto> buscarProductosPorNombre(const std::string& nombre);
    std::vector<Producto> buscarProductosPorMarca(const std::string& marca);
    std::vector<Producto> buscarProductosPorCategoria(const std::string& categoria);
    void mostrarTablaHashNombre() const;
    void mostrarTablaHashMarca() const;
    void mostrarTablaHashCategoria() const;

    bool agregarRelacionEntreProductos(int codigoOrigen, int codigoDestino, const std::string& tipoRelacion);
    std::vector<RelacionProducto> obtenerRelacionesDeProducto(int codigoProducto) const;
    void mostrarGrafoProductos() const;
};