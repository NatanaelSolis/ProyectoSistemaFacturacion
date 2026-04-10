#include "../../include/services/InventarioService.h"

InventarioService::InventarioService()
    : hashPorNombre(10), hashPorMarca(10), hashPorCategoria(10)
{
}

void InventarioService::indexarProductoEnHash(const Producto& producto)
{
    hashPorNombre.insertar(producto.getNombre(), producto);
    hashPorMarca.insertar(producto.getMarca(), producto);
    hashPorCategoria.insertar(producto.getCategoria(), producto);
    grafoProductos.agregarProducto(producto.getCodigo());
}

int InventarioService::cargarProductosIniciales()
{
    std::vector<Producto> productos = productoRepository.obtenerTodos();

    for (const Producto& producto : productos)
    {
        arbolProductos.insertarProducto(producto);
        indexarProductoEnHash(producto);
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
    indexarProductoEnHash(producto);
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

std::vector<Producto> InventarioService::buscarProductosPorNombre(const std::string& nombre)
{
    return hashPorNombre.buscar(nombre);
}

std::vector<Producto> InventarioService::buscarProductosPorMarca(const std::string& marca)
{
    return hashPorMarca.buscar(marca);
}

std::vector<Producto> InventarioService::buscarProductosPorCategoria(const std::string& categoria)
{
    return hashPorCategoria.buscar(categoria);
}

void InventarioService::mostrarTablaHashNombre() const
{
    hashPorNombre.mostrarTabla();
}

void InventarioService::mostrarTablaHashMarca() const
{
    hashPorMarca.mostrarTabla();
}

void InventarioService::mostrarTablaHashCategoria() const
{
    hashPorCategoria.mostrarTabla();
}

bool InventarioService::agregarRelacionEntreProductos(int codigoOrigen, int codigoDestino, const std::string& tipoRelacion)
{
    Producto* origen = arbolProductos.buscarProductoPorCodigo(codigoOrigen);
    Producto* destino = arbolProductos.buscarProductoPorCodigo(codigoDestino);

    if (origen == nullptr || destino == nullptr)
    {
        return false;
    }

    grafoProductos.agregarRelacion(codigoOrigen, codigoDestino, tipoRelacion);
    return true;
}

std::vector<RelacionProducto> InventarioService::obtenerRelacionesDeProducto(int codigoProducto) const
{
    return grafoProductos.obtenerRelaciones(codigoProducto);
}

void InventarioService::mostrarGrafoProductos() const
{
    grafoProductos.mostrarGrafo();
}