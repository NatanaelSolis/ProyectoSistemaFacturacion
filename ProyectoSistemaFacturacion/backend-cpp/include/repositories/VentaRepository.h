#pragma once

#include <vector>
#include "../models/Venta.h"
#include "../clients/ApiVentaClient.h"

class VentaRepository
{
private:
    ApiVentaClient apiVentaClient;

public:
    VentaRepository();

    bool registrarVenta(int clienteId, const std::vector<DetalleVenta>& detalles, Venta& ventaRegistrada);
    std::vector<Venta> obtenerVentas();
    bool obtenerVentaPorNumero(int numero, Venta& venta);
};