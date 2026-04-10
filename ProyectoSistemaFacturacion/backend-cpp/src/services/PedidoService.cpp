#include "../../include/services/PedidoService.h"

PedidoService::PedidoService()
    : siguienteNumeroPedido(1)
{
}

PedidoPendiente PedidoService::crearYEncolarPedido(
    int clienteId,
    const std::string& fecha,
    double total,
    const std::vector<DetalleVenta>& detalles)
{
    PedidoPendiente pedido(
        siguienteNumeroPedido,
        clienteId,
        fecha,
        "Pendiente",
        total
    );

    pedido.setDetalles(detalles);

    colaPedidos.encolarPedido(pedido);
    siguienteNumeroPedido++;

    return pedido;
}

bool PedidoService::atenderSiguientePedido(PedidoPendiente& pedidoAtendido)
{
    return colaPedidos.desencolarPedido(pedidoAtendido);
}

bool PedidoService::atenderSiguientePedidoYRegistrarVenta(
    PedidoPendiente& pedidoAtendido,
    Venta& ventaGenerada,
    VentaService& ventaService)
{
    PedidoPendiente pedidoFrente;

    if (!colaPedidos.desencolarPedido(pedidoFrente))
    {
        return false;
    }

    bool ventaRegistrada = ventaService.registrarVenta(
        pedidoFrente.getClienteId(),
        pedidoFrente.getDetalles(),
        ventaGenerada
    );

    if (!ventaRegistrada)
    {
        return false;
    }

    pedidoFrente.setEstado("Procesado");
    pedidoAtendido = pedidoFrente;

    return true;
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