#pragma once	

#include <string>
#include <iostream>

using namespace std;

class Cliente
{
private:
	int id;
	std::string nombre;
	std::string correo;
	std::string telefono;
	std::string direccion;
public:
    Cliente();
    Cliente(int id, const std::string& nombre, const std::string& correo,
        const std::string& telefono, const std::string& direccion);

    int getId() const;
    std::string getNombre() const;
    std::string getCorreo() const;
    std::string getTelefono() const;
    std::string getDireccion() const;

    void setId(int id);
    void setNombre(const std::string& nombre);
    void setCorreo(const std::string& correo);
    void setTelefono(const std::string& telefono);
    void setDireccion(const std::string& direccion);

    void mostrar() const;
};
