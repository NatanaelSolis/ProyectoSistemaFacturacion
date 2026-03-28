#pragma once

#include <vector>
#include "../models/Venta.h"
#include "NodoBMas.h"

class ArbolBMasVentas
{
private:
    NodoBMas* raiz;
    int grado;

    NodoBMas* buscarHoja(int clave);
    void insertarEnHoja(NodoBMas* hoja, const Venta& venta);
    void dividirHoja(NodoBMas* hoja);
    void insertarEnPadre(NodoBMas* izquierda, int clave, NodoBMas* derecha);
    void dividirInterno(NodoBMas* nodoInterno);
    void mostrarEnOrdenDesdeHoja(NodoBMas* hoja) const;
    void liberarMemoria(NodoBMas* nodo);

public:
    ArbolBMasVentas(int grado = 3);
    ~ArbolBMasVentas();

    void insertarVenta(const Venta& venta);
    Venta* buscarVentaPorNumero(int numero);
    void mostrarVentasEnOrden() const;
};