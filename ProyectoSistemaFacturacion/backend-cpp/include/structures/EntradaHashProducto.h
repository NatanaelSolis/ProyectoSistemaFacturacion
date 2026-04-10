#pragma once

#include <string>
#include <vector>
#include "../models/Producto.h"

class EntradaHashProducto
{
private:
    std::string clave;
    std::vector<Producto> productos;

public:
    EntradaHashProducto();
    EntradaHashProducto(const std::string& clave);

    std::string getClave() const;
    std::vector<Producto> getProductos() const;

    void setClave(const std::string& clave);
    void agregarProducto(const Producto& producto);
};