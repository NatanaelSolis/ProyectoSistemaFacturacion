#include "../../include/models/Venta.h"
#include <iostream>

Venta::Venta()
    : numero(0), fecha(""), clienteId(0), subtotal(0.0), iva(0.0), total(0.0), estado("")
{
}

Venta::Venta(int numero, const std::string& fecha, int clienteId, double subtotal, double iva, double total, const std::string& estado)
    : numero(numero), fecha(fecha), clienteId(clienteId), subtotal(subtotal), iva(iva), total(total), estado(estado)
{
}

int Venta::getNumero() const
{
    return numero;
}

std::string Venta::getFecha() const
{
    return fecha;
}

int Venta::getClienteId() const
{
    return clienteId;
}

double Venta::getSubtotal() const
{
    return subtotal;
}

double Venta::getIva() const
{
    return iva;
}

double Venta::getTotal() const
{
    return total;
}

std::string Venta::getEstado() const
{
    return estado;
}

std::vector<DetalleVenta> Venta::getDetalles() const
{
    return detalles;
}

void Venta::setNumero(int numero)
{
    this->numero = numero;
}

void Venta::setFecha(const std::string& fecha)
{
    this->fecha = fecha;
}

void Venta::setClienteId(int clienteId)
{
    this->clienteId = clienteId;
}

void Venta::setSubtotal(double subtotal)
{
    this->subtotal = subtotal;
}

void Venta::setIva(double iva)
{
    this->iva = iva;
}

void Venta::setTotal(double total)
{
    this->total = total;
}

void Venta::setEstado(const std::string& estado)
{
    this->estado = estado;
}

void Venta::setDetalles(const std::vector<DetalleVenta>& detalles)
{
    this->detalles = detalles;
}

void Venta::agregarDetalle(const DetalleVenta& detalle)
{
    detalles.push_back(detalle);
}

void Venta::mostrar() const
{
    std::cout << "Numero: " << numero << std::endl;
    std::cout << "Fecha: " << fecha << std::endl;
    std::cout << "Cliente ID: " << clienteId << std::endl;
    std::cout << "Subtotal sin IVA: " << subtotal << std::endl;
    std::cout << "IVA (13%): " << iva << std::endl;
    std::cout << "Total: " << total << std::endl;
    std::cout << "Estado: " << estado << std::endl;
    std::cout << "Detalles: " << detalles.size() << std::endl;
}