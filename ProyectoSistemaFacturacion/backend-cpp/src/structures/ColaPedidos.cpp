#include "../../include/structures/ColaPedidos.h"
#include <iostream>

ColaPedidos::ColaPedidos()
{
}

void ColaPedidos::encolarPedido(const PedidoPendiente& pedido)
{
    cola.push(pedido);
}

bool ColaPedidos::desencolarPedido(PedidoPendiente& pedidoAtendido)
{
    if (cola.empty())
    {
        return false;
    }

    pedidoAtendido = cola.front();
    cola.pop();
    return true;
}

bool ColaPedidos::verFrente(PedidoPendiente& pedidoFrente) const
{
    if (cola.empty())
    {
        return false;
    }

    pedidoFrente = cola.front();
    return true;
}

bool ColaPedidos::estaVacia() const
{
    return cola.empty();
}

int ColaPedidos::cantidadPedidos() const
{
    return static_cast<int>(cola.size());
}

void ColaPedidos::mostrarCola() const
{
    if (cola.empty())
    {
        std::cout << "No hay pedidos pendientes en la cola." << std::endl;
        return;
    }

    std::queue<PedidoPendiente> copia = cola;

    std::cout << "=== COLA DE PEDIDOS PENDIENTES ===" << std::endl;

    while (!copia.empty())
    {
        copia.front().mostrar();
        std::cout << "-----------------------------" << std::endl;
        copia.pop();
    }
}