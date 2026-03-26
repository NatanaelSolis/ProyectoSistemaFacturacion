#pragma once

#include <string>
#include <vector>
#include "../models/Producto.h"

class ApiProductoClient
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
    ApiProductoClient(
        const std::wstring& host = L"localhost",
        int puerto = 5251,
        bool usarHttps = false);

    std::vector<Producto> obtenerProductosIniciales() const;
    bool insertarProducto(const Producto& producto) const;
    bool eliminarProductoPorCodigo(int codigo) const;
};