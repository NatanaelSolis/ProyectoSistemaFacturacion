#include "../../include/repositories/ClienteRepository.h"

ClienteRepository::ClienteRepository()
    : apiClienteClient(L"localhost", 5251, false)
{
}

std::vector<Cliente> ClienteRepository::obtenerTodos()
{
    return apiClienteClient.obtenerClientesIniciales();
}

bool ClienteRepository::insertarCliente(const Cliente& cliente)
{
    return apiClienteClient.insertarCliente(cliente);
}

bool ClienteRepository::eliminarClientePorId(int id)
{
    return apiClienteClient.eliminarClientePorId(id);
}