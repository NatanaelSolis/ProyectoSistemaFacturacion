#include "../../include/models/DetalleVenta.h"

DetalleVenta::DetalleVenta()
    : id(0), productoCodigo(0), cantidad(0), precioUnitario(0.0), subtotal(0.0)
{
}

DetalleVenta::DetalleVenta(int id, int productoCodigo, int cantidad, double precioUnitario, double subtotal)
    : id(id), productoCodigo(productoCodigo), cantidad(cantidad), precioUnitario(precioUnitario), subtotal(subtotal)
{
}

int DetalleVenta::getId() const
{
    return id;
}

int DetalleVenta::getProductoCodigo() const
{
    return productoCodigo;
}

int DetalleVenta::getCantidad() const
{
    return cantidad;
}

double DetalleVenta::getPrecioUnitario() const
{
    return precioUnitario;
}

double DetalleVenta::getSubtotal() const
{
    return subtotal;
}

void DetalleVenta::setId(int id)
{
    this->id = id;
}

void DetalleVenta::setProductoCodigo(int productoCodigo)
{
    this->productoCodigo = productoCodigo;
}

void DetalleVenta::setCantidad(int cantidad)
{
    this->cantidad = cantidad;
}

void DetalleVenta::setPrecioUnitario(double precioUnitario)
{
    this->precioUnitario = precioUnitario;
}

void DetalleVenta::setSubtotal(double subtotal)
{
    this->subtotal = subtotal;
}