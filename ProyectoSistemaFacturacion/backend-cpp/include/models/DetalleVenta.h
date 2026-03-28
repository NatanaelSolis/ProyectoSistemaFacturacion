#pragma once

class DetalleVenta
{
private:
    int id;
    int productoCodigo;
    int cantidad;
    double precioUnitario;
    double subtotal;

public:
    DetalleVenta();
    DetalleVenta(int id, int productoCodigo, int cantidad, double precioUnitario, double subtotal);

    int getId() const;
    int getProductoCodigo() const;
    int getCantidad() const;
    double getPrecioUnitario() const;
    double getSubtotal() const;

    void setId(int id);
    void setProductoCodigo(int productoCodigo);
    void setCantidad(int cantidad);
    void setPrecioUnitario(double precioUnitario);
    void setSubtotal(double subtotal);
};