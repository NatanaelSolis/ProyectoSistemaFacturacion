#pragma once

#include <string>

class Producto
{
private:
    int codigo;
    std::string nombre;
    double precio;
    int cantidadStock;

public:
    Producto();
    Producto(int codigo, const std::string& nombre, double precio, int cantidadStock);

    int obtenerCodigo() const;
    std::string obtenerNombre() const;
    double obtenerPrecio() const;
    int obtenerCantidadStock() const;

    void establecerCodigo(int codigo);
    void establecerNombre(const std::string& nombre);
    void establecerPrecio(double precio);
    void establecerCantidadStock(int cantidadStock);

    std::string convertirATexto() const;
};