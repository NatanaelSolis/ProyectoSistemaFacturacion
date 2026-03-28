#include "../../include/services/VentaService.h"

#include <iostream>
#include <vector>

VentaService::VentaService()
{
}

bool VentaService::registrarVenta(int clienteId, const std::vector<DetalleVenta>& detalles, Venta& ventaRegistrada)
{
    bool registrada = ventaRepository.registrarVenta(clienteId, detalles, ventaRegistrada);

    if (!registrada)
    {
        return false;
    }

    ventas[ventaRegistrada.getNumero()] = ventaRegistrada;
    return true;
}

Venta* VentaService::buscarVentaPorNumero(int numero)
{
    auto it = ventas.find(numero);

    if (it != ventas.end())
    {
        return &(it->second);
    }

    Venta ventaApi;
    bool encontrada = ventaRepository.obtenerVentaPorNumero(numero, ventaApi);

    if (!encontrada)
    {
        return nullptr;
    }

    ventas[ventaApi.getNumero()] = ventaApi;
    return &ventas[ventaApi.getNumero()];
}

void VentaService::cargarVentasIniciales()
{
    std::vector<Venta> ventasApi = ventaRepository.obtenerVentas();

    for (const Venta& venta : ventasApi)
    {
        ventas[venta.getNumero()] = venta;
    }
}

void VentaService::mostrarVentas() const
{
    if (ventas.empty())
    {
        std::cout << "No hay ventas registradas." << std::endl;
        return;
    }

    for (const auto& par : ventas)
    {
        par.second.mostrar();
        std::cout << "-----------------------------" << std::endl;
    }
}