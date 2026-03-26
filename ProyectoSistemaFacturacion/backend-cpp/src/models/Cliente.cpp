#include "../../include/models/Cliente.h"

Cliente::Cliente()
    : id(0), nombre(""), correo(""), telefono(""), direccion("")
{
}

Cliente::Cliente(int id, const std::string& nombre, const std::string& correo,
    const std::string& telefono, const std::string& direccion)
    : id(id), nombre(nombre), correo(correo), telefono(telefono), direccion(direccion)
{
}

int Cliente::getId() const
{
    return id;
}

std::string Cliente::getNombre() const
{
    return nombre;
}

std::string Cliente::getCorreo() const
{
    return correo;
}

std::string Cliente::getTelefono() const
{
    return telefono;
}

std::string Cliente::getDireccion() const
{
    return direccion;
}

void Cliente::setId(int id)
{
    this->id = id;
}

void Cliente::setNombre(const std::string& nombre)
{
    this->nombre = nombre;
}

void Cliente::setCorreo(const std::string& correo)
{
    this->correo = correo;
}

void Cliente::setTelefono(const std::string& telefono)
{
    this->telefono = telefono;
}

void Cliente::setDireccion(const std::string& direccion)
{
    this->direccion = direccion;
}

void Cliente::mostrar() const
{
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Correo: " << correo << std::endl;
    std::cout << "Telefono: " << telefono << std::endl;
    std::cout << "Direccion: " << direccion << std::endl;
}