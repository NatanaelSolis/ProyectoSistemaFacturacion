#pragma once

#include <map>
#include "../models/Venta.h"
#include "../repositories/VentaRepository.h"
#include "../structures/ArbolBMasVentas.h"

class VentaService
{
private:
    std::map<int, Venta> ventas;
    VentaRepository ventaRepository;
    ArbolBMasVentas arbolBMasVentas;

public:
    VentaService();

    bool registrarVenta(int clienteId, const std::vector<DetalleVenta>& detalles, Venta& ventaRegistrada);
    Venta* buscarVentaPorNumero(int numero);
    void cargarVentasIniciales();
    void mostrarVentas() const;
};