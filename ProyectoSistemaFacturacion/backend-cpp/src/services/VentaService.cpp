#include "../../include/services/VentaService.h"

#include <iostream>
#include <vector>

VentaService::VentaService()
    : arbolBMasVentas(3)
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
    arbolBMasVentas.insertarVenta(ventaRegistrada);

    return true;
}

Venta* VentaService::buscarVentaPorNumero(int numero)
{
    Venta* ventaArbol = arbolBMasVentas.buscarVentaPorNumero(numero);

    if (ventaArbol != nullptr)
    {
        return ventaArbol;
    }

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
    arbolBMasVentas.insertarVenta(ventaApi);

    return arbolBMasVentas.buscarVentaPorNumero(ventaApi.getNumero());
}

void VentaService::cargarVentasIniciales()
{
    std::vector<Venta> ventasApi = ventaRepository.obtenerVentas();

    for (const Venta& venta : ventasApi)
    {
        ventas[venta.getNumero()] = venta;
        arbolBMasVentas.insertarVenta(venta);
    }
}

void VentaService::mostrarVentas() const
{
    arbolBMasVentas.mostrarVentasEnOrden();
}