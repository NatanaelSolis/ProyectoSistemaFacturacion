#pragma once

#include <map>
#include "../models/Cliente.h"
#include "../repositories/ClienteRepository.h"

class ClienteService
{
private:
    std::map<int, Cliente> clientes;
    ClienteRepository clienteRepository;

public:
    ClienteService();

    int cargarClientesIniciales();
    bool registrarCliente(const Cliente& cliente);
    Cliente* buscarClientePorId(int id);
    bool eliminarClientePorId(int id);
    void mostrarClientes() const;
};
