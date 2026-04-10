#pragma once

#include <map>
#include <vector>
#include <string>
#include "RelacionProducto.h"

class GrafoProductos
{
private:
    std::map<int, std::vector<RelacionProducto>> listaAdyacencia;

public:
    GrafoProductos();

    void agregarProducto(int codigoProducto);
    void agregarRelacion(int codigoOrigen, int codigoDestino, const std::string& tipoRelacion);
    std::vector<RelacionProducto> obtenerRelaciones(int codigoProducto) const;
    bool existeProducto(int codigoProducto) const;
    void mostrarGrafo() const;
};