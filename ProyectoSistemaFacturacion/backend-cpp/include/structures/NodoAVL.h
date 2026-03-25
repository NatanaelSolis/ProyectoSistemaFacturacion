#pragma once

#include "../models/Producto.h"

class NodoAVL
{
public:
    Producto producto;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(const Producto& producto);
};
