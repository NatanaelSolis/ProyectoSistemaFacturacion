#include "../../include/structures/RelacionProducto.h"

RelacionProducto::RelacionProducto()
    : codigoDestino(0), tipoRelacion("")
{
}

RelacionProducto::RelacionProducto(int codigoDestino, const std::string& tipoRelacion)
    : codigoDestino(codigoDestino), tipoRelacion(tipoRelacion)
{
}

int RelacionProducto::getCodigoDestino() const
{
    return codigoDestino;
}

std::string RelacionProducto::getTipoRelacion() const
{
    return tipoRelacion;
}

void RelacionProducto::setCodigoDestino(int codigoDestino)
{
    this->codigoDestino = codigoDestino;
}

void RelacionProducto::setTipoRelacion(const std::string& tipoRelacion)
{
    this->tipoRelacion = tipoRelacion;
}