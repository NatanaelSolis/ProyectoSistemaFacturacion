#pragma once

#include <vector>
#include "../models/Venta.h"

class NodoBMas
{
public:
    bool esHoja;
    std::vector<int> claves;
    std::vector<Venta> valores;
    std::vector<NodoBMas*> hijos;
    NodoBMas* siguiente;

    NodoBMas(bool esHoja)
        : esHoja(esHoja), siguiente(nullptr)
    {
    }
};
