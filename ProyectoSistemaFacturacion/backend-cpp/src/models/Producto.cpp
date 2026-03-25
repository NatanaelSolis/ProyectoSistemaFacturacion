#include "../../include/models/Producto.h"
#include <sstream>

// Constructor por defecto
Producto::Producto()
{
    this->codigo = 0;
    this->nombre = "";
    this->categoria = "";
    this->marca = "";
    this->precio = 0.0;
    this->stock = 0;
    this->estado = "Inactivo";
}

// Constructor con parámetros
Producto::Producto(int codigo, string nombre, string categoria, string marca, double precio, int stock, string estado)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->categoria = categoria;
    this->marca = marca;
    this->precio = precio;
    this->stock = stock;
    this->estado = estado;
}

// Getters
int Producto::getCodigo() const
{
    return codigo;
}

string Producto::getNombre() const
{
    return nombre;
}

string Producto::getCategoria() const
{
    return categoria;
}

string Producto::getMarca() const
{
    return marca;
}

double Producto::getPrecio() const
{
    return precio;
}

int Producto::getStock() const
{
    return stock;
}

string Producto::getEstado() const
{
    return estado;
}

// Setters
void Producto::setCodigo(int codigo)
{
    this->codigo = codigo;
}

void Producto::setNombre(string nombre)
{
    this->nombre = nombre;
}

void Producto::setCategoria(string categoria)
{
    this->categoria = categoria;
}

void Producto::setMarca(string marca)
{
    this->marca = marca;
}

void Producto::setPrecio(double precio)
{
    this->precio = precio;
}

void Producto::setStock(int stock)
{
    this->stock = stock;
}

void Producto::setEstado(string estado)
{
    this->estado = estado;
}

// Método para verificar si está activo
bool Producto::estaActivo() const
{
    return estado == "Activo";
}

// Mostrar en consola
void Producto::mostrar() const
{
    cout << "----------------------------------------" << endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Categoria: " << categoria << endl;
    cout << "Marca: " << marca << endl;
    cout << "Precio: " << precio << endl;
    cout << "Stock: " << stock << endl;
    cout << "Estado: " << estado << endl;
}

// COMPATIBILIDAD CON TU AVL
int Producto::obtenerCodigo() const
{
    return codigo;
}

string Producto::convertirATexto() const
{
    stringstream ss;
    ss << "Codigo: " << codigo
        << " | Nombre: " << nombre
        << " | Categoria: " << categoria
        << " | Marca: " << marca
        << " | Precio: " << precio
        << " | Stock: " << stock
        << " | Estado: " << estado;
    return ss.str();
}