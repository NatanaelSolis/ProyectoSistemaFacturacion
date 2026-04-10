#include "../../include/structures/TablaHashProductos.h"

#include <cctype>
#include <iostream>

TablaHashProductos::TablaHashProductos(int capacidadInicial)
    : capacidad(capacidadInicial), cantidadElementos(0)
{
    tabla.resize(capacidad);
}

int TablaHashProductos::funcionHash(const std::string& clave) const
{
    unsigned long hash = 5381;

    for (char c : clave)
    {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }

    return static_cast<int>(hash % capacidad);
}

double TablaHashProductos::factorCarga() const
{
    return static_cast<double>(cantidadElementos) / capacidad;
}

std::string TablaHashProductos::normalizarClave(const std::string& clave) const
{
    std::string resultado;

    for (char c : clave)
    {
        resultado += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    return resultado;
}

void TablaHashProductos::rehash()
{
    std::vector<std::vector<EntradaHashProducto>> tablaAnterior = tabla;

    capacidad *= 2;
    tabla.clear();
    tabla.resize(capacidad);
    cantidadElementos = 0;

    for (const auto& bucket : tablaAnterior)
    {
        for (const auto& entrada : bucket)
        {
            for (const auto& producto : entrada.getProductos())
            {
                insertar(entrada.getClave(), producto);
            }
        }
    }
}

void TablaHashProductos::insertar(const std::string& clave, const Producto& producto)
{
    std::string claveNormalizada = normalizarClave(clave);
    int indice = funcionHash(claveNormalizada);

    for (auto& entrada : tabla[indice])
    {
        if (entrada.getClave() == claveNormalizada)
        {
            entrada.agregarProducto(producto);
            return;
        }
    }

    EntradaHashProducto nuevaEntrada(claveNormalizada);
    nuevaEntrada.agregarProducto(producto);
    tabla[indice].push_back(nuevaEntrada);
    cantidadElementos++;

    if (factorCarga() > 0.75)
    {
        rehash();
    }
}

std::vector<Producto> TablaHashProductos::buscar(const std::string& clave) const
{
    std::string claveNormalizada = normalizarClave(clave);
    int indice = funcionHash(claveNormalizada);

    for (const auto& entrada : tabla[indice])
    {
        if (entrada.getClave() == claveNormalizada)
        {
            return entrada.getProductos();
        }
    }

    return {};
}

void TablaHashProductos::mostrarTabla() const
{
    std::cout << "=== TABLA HASH DE PRODUCTOS ===" << std::endl;

    for (int i = 0; i < capacidad; i++)
    {
        std::cout << "[" << i << "] ";

        if (tabla[i].empty())
        {
            std::cout << "vacio";
        }
        else
        {
            for (const auto& entrada : tabla[i])
            {
                std::cout << "{clave: " << entrada.getClave()
                    << ", productos: " << entrada.getProductos().size() << "} ";
            }
        }

        std::cout << std::endl;
    }
}