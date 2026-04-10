#include "../../include/services/PedidoService.h"

PedidoService::PedidoService()
    : siguienteNumeroPedido(1)
{
}

PedidoPendiente PedidoService::crearYEncolarPedido(int clienteId, const std::string& fecha, double total)
{
    PedidoPendiente pedido(
        siguienteNumeroPedido,
        clienteId,
        fecha,
        "Pendiente",
        total
    );

    colaPedidos.encolarPedido(pedido);
    siguienteNumeroPedido++;

    return pedido;
}

bool PedidoService::atenderSiguientePedido(PedidoPendiente& pedidoAtendido)
{
    return colaPedidos.desencolarPedido(pedidoAtendido);
}

bool PedidoService::verSiguientePedido(PedidoPendiente& pedidoFrente) const
{
    return colaPedidos.verFrente(pedidoFrente);
}

void PedidoService::mostrarPedidosPendientes() const
{
    colaPedidos.mostrarCola();
}

bool PedidoService::hayPedidosPendientes() const
{
    return !colaPedidos.estaVacia();
}