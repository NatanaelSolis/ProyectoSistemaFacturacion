#pragma once

#include <string>
#include <vector>
#include "EntradaHashProducto.h"

class TablaHashProductos
{
private:
    std::vector<std::vector<EntradaHashProducto>> tabla;
    int capacidad;
    int cantidadElementos;

    int funcionHash(const std::string& clave) const;
    double factorCarga() const;
    void rehash();
    std::string normalizarClave(const std::string& clave) const;

public:
    TablaHashProductos(int capacidadInicial = 10);

    void insertar(const std::string& clave, const Producto& producto);
    std::vector<Producto> buscar(const std::string& clave) const;
    void mostrarTabla() const;
};