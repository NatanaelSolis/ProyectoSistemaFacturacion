#pragma once

#include <string>

class RelacionProducto
{
private:
    int codigoDestino;
    std::string tipoRelacion;

public:
    RelacionProducto();
    RelacionProducto(int codigoDestino, const std::string& tipoRelacion);

    int getCodigoDestino() const;
    std::string getTipoRelacion() const;

    void setCodigoDestino(int codigoDestino);
    void setTipoRelacion(const std::string& tipoRelacion);
};