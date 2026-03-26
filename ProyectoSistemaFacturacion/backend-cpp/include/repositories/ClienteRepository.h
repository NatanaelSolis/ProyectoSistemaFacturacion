#pragma once

#include <vector>
#include "../models/Cliente.h"
#include "../clients/ApiClienteClient.h"

class ClienteRepository
{
private:
    ApiClienteClient apiClienteClient;

public:
    ClienteRepository();

    std::vector<Cliente> obtenerTodos();
    bool insertarCliente(const Cliente& cliente);
    bool eliminarClientePorId(int id);
};