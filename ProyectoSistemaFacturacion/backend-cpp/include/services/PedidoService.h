#pragma once

#include "../structures/ColaPedidos.h"

class PedidoService
{
private:
    ColaPedidos colaPedidos;
    int siguienteNumeroPedido;

public:
    PedidoService();

    PedidoPendiente crearYEncolarPedido(int clienteId, const std::string& fecha, double total);
    bool atenderSiguientePedido(PedidoPendiente& pedidoAtendido);
    bool verSiguientePedido(PedidoPendiente& pedidoFrente) const;
    void mostrarPedidosPendientes() const;
    bool hayPedidosPendientes() const;
};