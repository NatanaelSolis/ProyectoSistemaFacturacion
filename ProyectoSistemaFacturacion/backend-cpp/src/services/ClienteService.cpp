#include "../../include/services/ClienteService.h"

#include <iostream>
#include <vector>

ClienteService::ClienteService()
{
}

int ClienteService::cargarClientesIniciales()
{
    std::vector<Cliente> clientesIniciales = clienteRepository.obtenerTodos();

    for (const Cliente& cliente : clientesIniciales)
    {
        clientes[cliente.getId()] = cliente;
    }

    return static_cast<int>(clientesIniciales.size());
}

bool ClienteService::registrarCliente(const Cliente& cliente)
{
    if (clientes.find(cliente.getId()) != clientes.end())
    {
        return false;
    }

    bool insertadoEnApi = clienteRepository.insertarCliente(cliente);

    if (!insertadoEnApi)
    {
        return false;
    }

    clientes[cliente.getId()] = cliente;
    return true;
}

Cliente* ClienteService::buscarClientePorId(int id)
{
    auto it = clientes.find(id);

    if (it == clientes.end())
    {
        return nullptr;
    }

    return &(it->second);
}

bool ClienteService::eliminarClientePorId(int id)
{
    auto it = clientes.find(id);

    if (it == clientes.end())
    {
        return false;
    }

    bool eliminadoEnApi = clienteRepository.eliminarClientePorId(id);

    if (!eliminadoEnApi)
    {
        return false;
    }

    clientes.erase(it);
    return true;
}

void ClienteService::mostrarClientes() const
{
    if (clientes.empty())
    {
        std::cout << "No hay clientes registrados." << std::endl;
        return;
    }

    for (const auto& par : clientes)
    {
        par.second.mostrar();
        std::cout << "-----------------------------" << std::endl;
    }
}