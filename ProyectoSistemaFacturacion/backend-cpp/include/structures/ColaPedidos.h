#pragma once

#include <queue>
#include "../models/PedidoPendiente.h"

class ColaPedidos
{
private:
    std::queue<PedidoPendiente> cola;

public:
    ColaPedidos();

    void encolarPedido(const PedidoPendiente& pedido);
    bool desencolarPedido(PedidoPendiente& pedidoAtendido);
    bool verFrente(PedidoPendiente& pedidoFrente) const;
    bool estaVacia() const;
    int cantidadPedidos() const;
    void mostrarCola() const;
};