#include "../../include/repositories/VentaRepository.h"

VentaRepository::VentaRepository()
    : apiVentaClient(L"localhost", 5251, false)
{
}

bool VentaRepository::registrarVenta(int clienteId, const std::vector<DetalleVenta>& detalles, Venta& ventaRegistrada)
{
    return apiVentaClient.registrarVenta(clienteId, detalles, ventaRegistrada);
}

std::vector<Venta> VentaRepository::obtenerVentas()
{
    return apiVentaClient.obtenerVentas();
}

bool VentaRepository::obtenerVentaPorNumero(int numero, Venta& venta)
{
    return apiVentaClient.obtenerVentaPorNumero(numero, venta);
}