#pragma once

#include "NodoAVL.h"

class ArbolAVL
{
private:
    NodoAVL* raiz;

    int obtenerAltura(NodoAVL* nodo);
    int obtenerBalance(NodoAVL* nodo);
    int obtenerMayor(int a, int b);

    NodoAVL* rotarDerecha(NodoAVL* nodo);
    NodoAVL* rotarIzquierda(NodoAVL* nodo);

    NodoAVL* insertar(NodoAVL* nodo, const Producto& producto);
    NodoAVL* buscar(NodoAVL* nodo, int codigo);
    void mostrarEnOrden(NodoAVL* nodo);

    NodoAVL* obtenerNodoMinimo(NodoAVL* nodo);
    NodoAVL* eliminar(NodoAVL* nodo, int codigo);


public:
    ArbolAVL();

    void insertarProducto(const Producto& producto);
    Producto* buscarProductoPorCodigo(int codigo);
    void mostrarEnOrden();

    void eliminarProductoPorCodigo(int codigo);

};