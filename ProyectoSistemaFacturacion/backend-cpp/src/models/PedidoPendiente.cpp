#include "../../include/models/PedidoPendiente.h"
#include <iostream>

PedidoPendiente::PedidoPendiente()
    : numero(0), clienteId(0), fecha(""), estado("Pendiente"), total(0.0)
{
}

PedidoPendiente::PedidoPendiente(int numero, int clienteId, const std::string& fecha, const std::string& estado, double total)
    : numero(numero), clienteId(clienteId), fecha(fecha), estado(estado), total(total)
{
}

int PedidoPendiente::getNumero() const
{
    return numero;
}

int PedidoPendiente::getClienteId() const
{
    return clienteId;
}

std::string PedidoPendiente::getFecha() const
{
    return fecha;
}

std::string PedidoPendiente::getEstado() const
{
    return estado;
}

double PedidoPendiente::getTotal() const
{
    return total;
}

void PedidoPendiente::setNumero(int numero)
{
    this->numero = numero;
}

void PedidoPendiente::setClienteId(int clienteId)
{
    this->clienteId = clienteId;
}

void PedidoPendiente::setFecha(const std::string& fecha)
{
    this->fecha = fecha;
}

void PedidoPendiente::setEstado(const std::string& estado)
{
    this->estado = estado;
}

void PedidoPendiente::setTotal(double total)
{
    this->total = total;
}

void PedidoPendiente::mostrar() const
{
    std::cout << "Numero de pedido: " << numero << std::endl;
    std::cout << "Cliente ID: " << clienteId << std::endl;
    std::cout << "Fecha: " << fecha << std::endl;
    std::cout << "Estado: " << estado << std::endl;
    std::cout << "Total: " << total << std::endl;
}