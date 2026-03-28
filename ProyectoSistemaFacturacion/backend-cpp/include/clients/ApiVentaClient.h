#pragma once

#include <string>
#include <vector>
#include "../models/Venta.h"

class ApiVentaClient
{
private:
    std::wstring host;
    int puerto;
    bool usarHttps;

    bool realizarSolicitud(
        const std::wstring& metodo,
        const std::wstring& ruta,
        const std::string& cuerpo,
        long& codigoEstado,
        std::string& respuesta) const;

public:
    ApiVentaClient(
        const std::wstring& host = L"localhost",
        int puerto = 5251,
        bool usarHttps = false);

    bool registrarVenta(int clienteId, const std::vector<DetalleVenta>& detalles, Venta& ventaRegistrada) const;
    std::vector<Venta> obtenerVentas() const;
    bool obtenerVentaPorNumero(int numero, Venta& venta) const;
};