#include "../../include/models/Producto.h"

#include <sstream>

// Constructor vacío
Producto::Producto()
{
    codigo = 0;
    nombre = "";
    precio = 0.0;
    cantidadStock = 0;
}

// Constructor con parámetros
Producto::Producto(int codigo, const std::string& nombre, double precio, int cantidadStock)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->precio = precio;
    this->cantidadStock = cantidadStock;
}

// Getters
int Producto::obtenerCodigo() const
{
    return codigo;
}

std::string Producto::obtenerNombre() const
{
    return nombre;
}

double Producto::obtenerPrecio() const
{
    return precio;
}

int Producto::obtenerCantidadStock() const
{
    return cantidadStock;
}

// Setters
void Producto::establecerCodigo(int codigo)
{
    this->codigo = codigo;
}

void Producto::establecerNombre(const std::string& nombre)
{
    this->nombre = nombre;
}

void Producto::establecerPrecio(double precio)
{
    this->precio = precio;
}

void Producto::establecerCantidadStock(int cantidadStock)
{
    this->cantidadStock = cantidadStock;
}

// Método para convertir la información del producto en texto
std::string Producto::convertirATexto() const
{
    std::ostringstream salida;
    salida << "Codigo: " << codigo
        << " | Nombre: " << nombre
        << " | Precio: " << precio
        << " | Stock: " << cantidadStock;

    return salida.str();
}