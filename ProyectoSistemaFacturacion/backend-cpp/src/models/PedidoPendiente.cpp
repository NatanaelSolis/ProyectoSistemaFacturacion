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

std::vector<DetalleVenta> PedidoPendiente::getDetalles() const
{
    return detalles;
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

void PedidoPendiente::setDetalles(const std::vector<DetalleVenta>& detalles)
{
    this->detalles = detalles;
}

void PedidoPendiente::agregarDetalle(const DetalleVenta& detalle)
{
    detalles.push_back(detalle);
}

void PedidoPendiente::mostrar() const
{
    std::cout << "Numero de pedido: " << numero << std::endl;
    std::cout << "Cliente ID: " << clienteId << std::endl;
    std::cout << "Fecha: " << fecha << std::endl;
    std::cout << "Estado: " << estado << std::endl;
    std::cout << "Total: " << total << std::endl;
    std::cout << "Cantidad de lineas: " << detalles.size() << std::endl;

    if (!detalles.empty())
    {
        std::cout << "Detalles del pedido:" << std::endl;

        for (size_t i = 0; i < detalles.size(); i++)
        {
            std::cout << "  Linea #" << (i + 1) << std::endl;
            std::cout << "  Producto codigo: " << detalles[i].getProductoCodigo() << std::endl;
            std::cout << "  Cantidad: " << detalles[i].getCantidad() << std::endl;
            std::cout << "-----------------------------" << std::endl;
        }
    }
}