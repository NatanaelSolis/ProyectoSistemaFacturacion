#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "../../include/clients/ApiClienteClient.h"

#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#include <iostream>
#include <sstream>
#include <vector>

namespace
{
    std::string recortar(const std::string& texto)
    {
        const std::string espacios = " \t\r\n";
        const size_t inicio = texto.find_first_not_of(espacios);

        if (inicio == std::string::npos)
        {
            return "";
        }

        const size_t fin = texto.find_last_not_of(espacios);
        return texto.substr(inicio, fin - inicio + 1);
    }

    std::vector<std::string> dividir(const std::string& texto, char delimitador)
    {
        std::vector<std::string> partes;
        std::string parte;
        std::stringstream ss(texto);

        while (std::getline(ss, parte, delimitador))
        {
            partes.push_back(parte);
        }

        return partes;
    }

    std::string escaparJson(const std::string& texto)
    {
        std::string resultado;

        for (char c : texto)
        {
            switch (c)
            {
            case '\\':
                resultado += "\\\\";
                break;
            case '"':
                resultado += "\\\"";
                break;
            case '\n':
                resultado += "\\n";
                break;
            case '\r':
                break;
            case '\t':
                resultado += "\\t";
                break;
            default:
                resultado += c;
                break;
            }
        }

        return resultado;
    }
}

ApiClienteClient::ApiClienteClient(const std::wstring& host, int puerto, bool usarHttps)
    : host(host), puerto(puerto), usarHttps(usarHttps)
{
}

bool ApiClienteClient::realizarSolicitud(
    const std::wstring& metodo,
    const std::wstring& ruta,
    const std::string& cuerpo,
    long& codigoEstado,
    std::string& respuesta) const
{
    codigoEstado = 0;
    respuesta.clear();

    HINTERNET hSesion = WinHttpOpen(
        L"ProyectoSistemaFacturacion/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (hSesion == nullptr)
    {
        std::cerr << "[ERROR] No se pudo abrir la sesion WinHTTP." << std::endl;
        return false;
    }

    HINTERNET hConexion = WinHttpConnect(
        hSesion,
        host.c_str(),
        static_cast<INTERNET_PORT>(puerto),
        0);

    if (hConexion == nullptr)
    {
        std::cerr << "[ERROR] No se pudo conectar con la API." << std::endl;
        WinHttpCloseHandle(hSesion);
        return false;
    }

    DWORD flags = usarHttps ? WINHTTP_FLAG_SECURE : 0;

    HINTERNET hSolicitud = WinHttpOpenRequest(
        hConexion,
        metodo.c_str(),
        ruta.c_str(),
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        flags);

    if (hSolicitud == nullptr)
    {
        std::cerr << "[ERROR] No se pudo abrir la solicitud HTTP." << std::endl;
        WinHttpCloseHandle(hConexion);
        WinHttpCloseHandle(hSesion);
        return false;
    }

    std::wstring cabeceras = L"";
    if (!cuerpo.empty())
    {
        cabeceras = L"Content-Type: application/json; charset=utf-8\r\n";
    }

    BOOL enviado = WinHttpSendRequest(
        hSolicitud,
        cabeceras.empty() ? WINHTTP_NO_ADDITIONAL_HEADERS : cabeceras.c_str(),
        cabeceras.empty() ? 0 : static_cast<DWORD>(-1),
        cuerpo.empty() ? WINHTTP_NO_REQUEST_DATA : (LPVOID)cuerpo.data(),
        cuerpo.empty() ? 0 : static_cast<DWORD>(cuerpo.size()),
        cuerpo.empty() ? 0 : static_cast<DWORD>(cuerpo.size()),
        0);

    if (!enviado)
    {
        std::cerr << "[ERROR] No se pudo enviar la solicitud HTTP." << std::endl;
        WinHttpCloseHandle(hSolicitud);
        WinHttpCloseHandle(hConexion);
        WinHttpCloseHandle(hSesion);
        return false;
    }

    BOOL recibida = WinHttpReceiveResponse(hSolicitud, nullptr);

    if (!recibida)
    {
        std::cerr << "[ERROR] No se pudo recibir la respuesta de la API." << std::endl;
        WinHttpCloseHandle(hSolicitud);
        WinHttpCloseHandle(hConexion);
        WinHttpCloseHandle(hSesion);
        return false;
    }

    DWORD estado = 0;
    DWORD tamanoEstado = sizeof(estado);

    WinHttpQueryHeaders(
        hSolicitud,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX,
        &estado,
        &tamanoEstado,
        WINHTTP_NO_HEADER_INDEX);

    codigoEstado = static_cast<long>(estado);

    DWORD tamanoDisponible = 0;

    do
    {
        tamanoDisponible = 0;

        if (!WinHttpQueryDataAvailable(hSolicitud, &tamanoDisponible))
        {
            std::cerr << "[ERROR] No se pudo consultar el tamano de la respuesta." << std::endl;
            break;
        }

        if (tamanoDisponible == 0)
        {
            break;
        }

        std::string buffer;
        buffer.resize(tamanoDisponible);

        DWORD leidos = 0;

        if (!WinHttpReadData(hSolicitud, &buffer[0], tamanoDisponible, &leidos))
        {
            std::cerr << "[ERROR] No se pudo leer la respuesta de la API." << std::endl;
            break;
        }

        buffer.resize(leidos);
        respuesta += buffer;

    } while (tamanoDisponible > 0);

    WinHttpCloseHandle(hSolicitud);
    WinHttpCloseHandle(hConexion);
    WinHttpCloseHandle(hSesion);

    return true;
}

std::vector<Cliente> ApiClienteClient::obtenerClientesIniciales() const
{
    std::vector<Cliente> clientes;
    long codigoEstado = 0;
    std::string respuesta;

    bool ok = realizarSolicitud(
        L"GET",
        L"/api/Clientes/carga-inicial",
        "",
        codigoEstado,
        respuesta);

    if (!ok || codigoEstado != 200)
    {
        std::cerr << "[ERROR] No se pudo obtener la carga inicial de clientes. Codigo HTTP: " << codigoEstado << std::endl;
        return clientes;
    }

    std::string linea;
    std::stringstream ss(respuesta);

    while (std::getline(ss, linea))
    {
        linea = recortar(linea);

        if (linea.empty())
        {
            continue;
        }

        std::vector<std::string> campos = dividir(linea, ';');

        if (campos.size() != 5)
        {
            std::cerr << "[ADVERTENCIA] Linea de cliente ignorada por formato invalido: " << linea << std::endl;
            continue;
        }

        try
        {
            int id = std::stoi(recortar(campos[0]));
            std::string nombre = recortar(campos[1]);
            std::string correo = recortar(campos[2]);
            std::string telefono = recortar(campos[3]);
            std::string direccion = recortar(campos[4]);

            clientes.push_back(Cliente(id, nombre, correo, telefono, direccion));
        }
        catch (const std::exception&)
        {
            std::cerr << "[ADVERTENCIA] No se pudo convertir una linea de cliente: " << linea << std::endl;
        }
    }

    return clientes;
}

bool ApiClienteClient::insertarCliente(const Cliente& cliente) const
{
    long codigoEstado = 0;
    std::string respuesta;

    std::ostringstream json;

    json << "{"
        << "\"id\":" << cliente.getId() << ","
        << "\"nombre\":\"" << escaparJson(cliente.getNombre()) << "\","
        << "\"correo\":\"" << escaparJson(cliente.getCorreo()) << "\","
        << "\"telefono\":\"" << escaparJson(cliente.getTelefono()) << "\","
        << "\"direccion\":\"" << escaparJson(cliente.getDireccion()) << "\""
        << "}";

    bool ok = realizarSolicitud(
        L"POST",
        L"/api/Clientes",
        json.str(),
        codigoEstado,
        respuesta);

    if (!ok)
    {
        return false;
    }

    if (codigoEstado == 201 || codigoEstado == 200)
    {
        return true;
    }

    std::cerr << "[ERROR] La API no pudo insertar el cliente. Codigo HTTP: " << codigoEstado << std::endl;
    if (!respuesta.empty())
    {
        std::cerr << "Detalle: " << respuesta << std::endl;
    }

    return false;
}

bool ApiClienteClient::eliminarClientePorId(int id) const
{
    long codigoEstado = 0;
    std::string respuesta;

    std::wstring ruta = L"/api/Clientes/" + std::to_wstring(id);

    bool ok = realizarSolicitud(
        L"DELETE",
        ruta,
        "",
        codigoEstado,
        respuesta);

    if (!ok)
    {
        return false;
    }

    if (codigoEstado == 204 || codigoEstado == 200)
    {
        return true;
    }

    std::cerr << "[ERROR] La API no pudo eliminar el cliente. Codigo HTTP: " << codigoEstado << std::endl;
    if (!respuesta.empty())
    {
        std::cerr << "Detalle: " << respuesta << std::endl;
    }

    return false;
}