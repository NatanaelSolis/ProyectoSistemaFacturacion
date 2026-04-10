#pragma once

#include "../structures/ColaPedidos.h"
#include "VentaService.h"

class PedidoService
{
private:
    ColaPedidos colaPedidos;
    int siguienteNumeroPedido;

public:
    PedidoService();

    PedidoPendiente crearYEncolarPedido(
        int clienteId,
        const std::string& fecha,
        double total,
        const std::vector<DetalleVenta>& detalles
    );

    bool atenderSiguientePedido(PedidoPendiente& pedidoAtendido);
    bool atenderSiguientePedidoYRegistrarVenta(PedidoPendiente& pedidoAtendido, Venta& ventaGenerada, VentaService& ventaService);
    bool verSiguientePedido(PedidoPendiente& pedidoFrente) const;
    void mostrarPedidosPendientes() const;
    bool hayPedidosPendientes() const;
};