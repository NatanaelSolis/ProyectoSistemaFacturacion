#pragma once

#include <string>
#include <vector>
#include "DetalleVenta.h"

class Venta
{
private:
    int numero;
    std::string fecha;
    int clienteId;
    double subtotal;
    double iva;
    double total;
    std::string estado;
    std::vector<DetalleVenta> detalles;

public:
    Venta();
    Venta(int numero, const std::string& fecha, int clienteId, double subtotal, double iva, double total, const std::string& estado);

    int getNumero() const;
    std::string getFecha() const;
    int getClienteId() const;
    double getSubtotal() const;
    double getIva() const;
    double getTotal() const;
    std::string getEstado() const;
    std::vector<DetalleVenta> getDetalles() const;

    void setNumero(int numero);
    void setFecha(const std::string& fecha);
    void setClienteId(int clienteId);
    void setSubtotal(double subtotal);
    void setIva(double iva);
    void setTotal(double total);
    void setEstado(const std::string& estado);
    void setDetalles(const std::vector<DetalleVenta>& detalles);

    void agregarDetalle(const DetalleVenta& detalle);
    void mostrar() const;
};