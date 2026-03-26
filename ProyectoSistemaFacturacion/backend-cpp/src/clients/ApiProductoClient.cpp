#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "../../include/clients/ApiProductoClient.h"

#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <locale>

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

ApiProductoClient::ApiProductoClient(const std::wstring& host, int puerto, bool usarHttps)
    : host(host), puerto(puerto), usarHttps(usarHttps)
{
}

bool ApiProductoClient::realizarSolicitud(
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
            std::cerr << "[ERROR] No se pudo consultar el tamaño de la respuesta." << std::endl;
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

std::vector<Producto> ApiProductoClient::obtenerProductosIniciales() const
{
    std::vector<Producto> productos;
    long codigoEstado = 0;
    std::string respuesta;

    bool ok = realizarSolicitud(
        L"GET",
        L"/api/Productos/carga-inicial",
        "",
        codigoEstado,
        respuesta);

    if (!ok || codigoEstado != 200)
    {
        std::cerr << "[ERROR] No se pudo obtener la carga inicial desde la API. Codigo HTTP: " << codigoEstado << std::endl;
        return productos;
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

        if (campos.size() != 7)
        {
            std::cerr << "[ADVERTENCIA] Linea ignorada por formato invalido: " << linea << std::endl;
            continue;
        }

        try
        {
            int codigo = std::stoi(recortar(campos[0]));
            std::string nombre = recortar(campos[1]);
            std::string categoria = recortar(campos[2]);
            std::string marca = recortar(campos[3]);
            double precio = std::stod(recortar(campos[4]));
            int stock = std::stoi(recortar(campos[5]));
            std::string estado = recortar(campos[6]);

            productos.push_back(Producto(codigo, nombre, categoria, marca, precio, stock, estado));
        }
        catch (const std::exception&)
        {
            std::cerr << "[ADVERTENCIA] No se pudo convertir una linea de la respuesta: " << linea << std::endl;
        }
    }

    return productos;
}

bool ApiProductoClient::insertarProducto(const Producto& producto) const
{
    long codigoEstado = 0;
    std::string respuesta;

    std::ostringstream json;
    json.imbue(std::locale::classic());

    json << "{"
        << "\"codigo\":" << producto.getCodigo() << ","
        << "\"nombre\":\"" << escaparJson(producto.getNombre()) << "\","
        << "\"categoria\":\"" << escaparJson(producto.getCategoria()) << "\","
        << "\"marca\":\"" << escaparJson(producto.getMarca()) << "\","
        << "\"precio\":" << std::fixed << std::setprecision(2) << producto.getPrecio() << ","
        << "\"stock\":" << producto.getStock() << ","
        << "\"estado\":\"" << escaparJson(producto.getEstado()) << "\""
        << "}";

    bool ok = realizarSolicitud(
        L"POST",
        L"/api/Productos",
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

    std::cerr << "[ERROR] La API no pudo insertar el producto. Codigo HTTP: " << codigoEstado << std::endl;
    if (!respuesta.empty())
    {
        std::cerr << "Detalle: " << respuesta << std::endl;
    }

    return false;
}

bool ApiProductoClient::eliminarProductoPorCodigo(int codigo) const
{
    long codigoEstado = 0;
    std::string respuesta;

    std::wstring ruta = L"/api/Productos/" + std::to_wstring(codigo);

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

    std::cerr << "[ERROR] La API no pudo eliminar el producto. Codigo HTTP: " << codigoEstado << std::endl;
    if (!respuesta.empty())
    {
        std::cerr << "Detalle: " << respuesta << std::endl;
    }

    return false;
}