#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "../../include/clients/ApiVentaClient.h"

#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

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

    std::string extraerValor(const std::string& json, const std::string& clave)
    {
        std::string patron = "\"" + clave + "\":";
        size_t pos = json.find(patron);

        if (pos == std::string::npos)
        {
            return "";
        }

        pos += patron.length();

        while (pos < json.size() && std::isspace(static_cast<unsigned char>(json[pos])))
        {
            pos++;
        }

        if (pos >= json.size())
        {
            return "";
        }

        if (json[pos] == '"')
        {
            pos++;
            size_t fin = json.find('"', pos);
            if (fin == std::string::npos)
            {
                return "";
            }
            return json.substr(pos, fin - pos);
        }

        size_t fin = pos;
        while (fin < json.size() &&
            json[fin] != ',' &&
            json[fin] != '}' &&
            json[fin] != '\n' &&
            json[fin] != '\r')
        {
            fin++;
        }

        return recortar(json.substr(pos, fin - pos));
    }

    std::vector<std::string> dividirObjetosJson(const std::string& json)
    {
        std::vector<std::string> objetos;
        int nivelLlaves = 0;
        bool dentroObjeto = false;
        std::string actual;

        for (char c : json)
        {
            if (c == '{')
            {
                nivelLlaves++;
                dentroObjeto = true;
            }

            if (dentroObjeto)
            {
                actual += c;
            }

            if (c == '}')
            {
                nivelLlaves--;

                if (nivelLlaves == 0 && dentroObjeto)
                {
                    objetos.push_back(actual);
                    actual.clear();
                    dentroObjeto = false;
                }
            }
        }

        return objetos;
    }

    Venta parsearVentaSimple(const std::string& json)
    {
        Venta venta;

        std::string numero = extraerValor(json, "numero");
        std::string fecha = extraerValor(json, "fecha");
        std::string clienteId = extraerValor(json, "clienteId");
        std::string subtotal = extraerValor(json, "subtotal");
        std::string iva = extraerValor(json, "IVA");
        std::string total = extraerValor(json, "total");
        std::string estado = extraerValor(json, "estado");

        if (!numero.empty()) venta.setNumero(std::stoi(numero));
        if (!fecha.empty()) venta.setFecha(fecha);
        if (!clienteId.empty()) venta.setClienteId(std::stoi(clienteId));
        if (!subtotal.empty()) venta.setSubtotal(std::stod(subtotal));
        if (!iva.empty()) venta.setIva(std::stod(iva));
        if (!total.empty()) venta.setTotal(std::stod(total));
        if (!estado.empty()) venta.setEstado(estado);

        return venta;
    }
}

ApiVentaClient::ApiVentaClient(const std::wstring& host, int puerto, bool usarHttps)
    : host(host), puerto(puerto), usarHttps(usarHttps)
{
}

bool ApiVentaClient::realizarSolicitud(
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

bool ApiVentaClient::registrarVenta(int clienteId, const std::vector<DetalleVenta>& detalles, Venta& ventaRegistrada) const
{
    long codigoEstado = 0;
    std::string respuesta;

    std::ostringstream json;
    json << "{";
    json << "\"clienteId\":" << clienteId << ",";
    json << "\"detalles\":[";

    for (size_t i = 0; i < detalles.size(); i++)
    {
        json << "{";
        json << "\"productoCodigo\":" << detalles[i].getProductoCodigo() << ",";
        json << "\"cantidad\":" << detalles[i].getCantidad();
        json << "}";

        if (i + 1 < detalles.size())
        {
            json << ",";
        }
    }

    json << "]";
    json << "}";

    bool ok = realizarSolicitud(
        L"POST",
        L"/api/Ventas",
        json.str(),
        codigoEstado,
        respuesta);

    if (!ok)
    {
        return false;
    }

    if (codigoEstado == 201 || codigoEstado == 200)
    {
        ventaRegistrada = parsearVentaSimple(respuesta);
        return true;
    }

    std::cerr << "[ERROR] La API no pudo registrar la venta. Codigo HTTP: " << codigoEstado << std::endl;
    if (!respuesta.empty())
    {
        std::cerr << "Detalle: " << respuesta << std::endl;
    }

    return false;
}

std::vector<Venta> ApiVentaClient::obtenerVentas() const
{
    std::vector<Venta> ventas;
    long codigoEstado = 0;
    std::string respuesta;

    bool ok = realizarSolicitud(
        L"GET",
        L"/api/Ventas",
        "",
        codigoEstado,
        respuesta);

    if (!ok || codigoEstado != 200)
    {
        std::cerr << "[ERROR] No se pudieron obtener las ventas. Codigo HTTP: " << codigoEstado << std::endl;
        return ventas;
    }

    std::vector<std::string> objetos = dividirObjetosJson(respuesta);

    for (const std::string& objeto : objetos)
    {
        if (objeto.find("\"numero\"") != std::string::npos &&
            objeto.find("\"clienteId\"") != std::string::npos)
        {
            ventas.push_back(parsearVentaSimple(objeto));
        }
    }

    return ventas;
}

bool ApiVentaClient::obtenerVentaPorNumero(int numero, Venta& venta) const
{
    long codigoEstado = 0;
    std::string respuesta;

    std::wstring ruta = L"/api/Ventas/" + std::to_wstring(numero);

    bool ok = realizarSolicitud(
        L"GET",
        ruta,
        "",
        codigoEstado,
        respuesta);

    if (!ok)
    {
        return false;
    }

    if (codigoEstado == 200)
    {
        venta = parsearVentaSimple(respuesta);
        return true;
    }

    std::cerr << "[ERROR] No se pudo obtener la venta. Codigo HTTP: " << codigoEstado << std::endl;
    if (!respuesta.empty())
    {
        std::cerr << "Detalle: " << respuesta << std::endl;
    }

    return false;
}