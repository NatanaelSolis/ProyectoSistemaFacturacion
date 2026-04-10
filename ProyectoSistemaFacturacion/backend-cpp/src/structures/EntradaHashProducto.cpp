#include "../../include/structures/EntradaHashProducto.h"

EntradaHashProducto::EntradaHashProducto()
    : clave("")
{
}

EntradaHashProducto::EntradaHashProducto(const std::string& clave)
    : clave(clave)
{
}

std::string EntradaHashProducto::getClave() const
{
    return clave;
}

std::vector<Producto> EntradaHashProducto::getProductos() const
{
    return productos;
}

void EntradaHashProducto::setClave(const std::string& clave)
{
    this->clave = clave;
}

void EntradaHashProducto::agregarProducto(const Producto& producto)
{
    productos.push_back(producto);
}