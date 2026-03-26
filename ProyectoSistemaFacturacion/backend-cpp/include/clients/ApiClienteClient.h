#pragma once

#include <string>
#include <vector>
#include "../models/Cliente.h"

class ApiClienteClient
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
    ApiClienteClient(
        const std::wstring& host = L"localhost",
        int puerto = 5251,
        bool usarHttps = false);

    std::vector<Cliente> obtenerClientesIniciales() const;
    bool insertarCliente(const Cliente& cliente) const;
    bool eliminarClientePorId(int id) const;
};
