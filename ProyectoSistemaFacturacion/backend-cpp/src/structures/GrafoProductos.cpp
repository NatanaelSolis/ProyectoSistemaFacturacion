#include "../../include/structures/GrafoProductos.h"
#include <iostream>

GrafoProductos::GrafoProductos()
{
}

void GrafoProductos::agregarProducto(int codigoProducto)
{
    if (!existeProducto(codigoProducto))
    {
        listaAdyacencia[codigoProducto] = std::vector<RelacionProducto>();
    }
}

void GrafoProductos::agregarRelacion(int codigoOrigen, int codigoDestino, const std::string& tipoRelacion)
{
    agregarProducto(codigoOrigen);
    agregarProducto(codigoDestino);

    listaAdyacencia[codigoOrigen].push_back(RelacionProducto(codigoDestino, tipoRelacion));
}

std::vector<RelacionProducto> GrafoProductos::obtenerRelaciones(int codigoProducto) const
{
    auto it = listaAdyacencia.find(codigoProducto);

    if (it != listaAdyacencia.end())
    {
        return it->second;
    }

    return {};
}

bool GrafoProductos::existeProducto(int codigoProducto) const
{
    return listaAdyacencia.find(codigoProducto) != listaAdyacencia.end();
}

void GrafoProductos::mostrarGrafo() const
{
    if (listaAdyacencia.empty())
    {
        std::cout << "El grafo de productos esta vacio." << std::endl;
        return;
    }

    std::cout << "=== GRAFO DE PRODUCTOS RELACIONADOS ===" << std::endl;

    for (const auto& par : listaAdyacencia)
    {
        std::cout << "Producto " << par.first << " -> ";

        if (par.second.empty())
        {
            std::cout << "sin relaciones";
        }
        else
        {
            for (const auto& relacion : par.second)
            {
                std::cout << "[" << relacion.getCodigoDestino()
                    << " | " << relacion.getTipoRelacion() << "] ";
            }
        }

        std::cout << std::endl;
    }
}