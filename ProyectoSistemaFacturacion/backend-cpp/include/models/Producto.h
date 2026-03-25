#pragma once
#include <string>
#include <iostream>

using namespace std;

class Producto
{
private:
    int codigo;
    string nombre;
    string categoria;
    string marca;
    double precio;
    int stock;
    string estado;

public:
    // Constructores
    Producto();
    Producto(int codigo, string nombre, string categoria, string marca, double precio, int stock, string estado);

    // Getters tradicionales
    int getCodigo() const;
    string getNombre() const;
    string getCategoria() const;
    string getMarca() const;
    double getPrecio() const;
    int getStock() const;
    string getEstado() const;

    // Setters
    void setCodigo(int codigo);
    void setNombre(string nombre);
    void setCategoria(string categoria);
    void setMarca(string marca);
    void setPrecio(double precio);
    void setStock(int stock);
    void setEstado(string estado);

    // Métodos auxiliares
    bool estaActivo() const;
    void mostrar() const;

    // MÉTODOS DE COMPATIBILIDAD CON TU AVL ACTUAL
    int obtenerCodigo() const;
    string convertirATexto() const;
};