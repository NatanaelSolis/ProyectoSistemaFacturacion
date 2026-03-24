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
    // Constructores
    Producto();
    Producto(int codigo, const std::string& nombre, double precio, int cantidadStock);

    // Getters
    int obtenerCodigo() const;
    std::string obtenerNombre() const;
    double obtenerPrecio() const;
    int obtenerCantidadStock() const;

    // Setters
    void establecerCodigo(int codigo);
    void establecerNombre(const std::string& nombre);
    void establecerPrecio(double precio);
    void establecerCantidadStock(int cantidadStock);

    // Método utilitario para mostrar la información del producto
    std::string convertirATexto() const;
};