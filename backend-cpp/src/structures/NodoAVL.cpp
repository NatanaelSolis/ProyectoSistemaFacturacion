#include "../../include/structures/NodoAVL.h"

NodoAVL::NodoAVL(const Producto& producto)
{
    this->producto = producto;
    izquierdo = nullptr;
    derecho = nullptr;
    altura = 1;
}