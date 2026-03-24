#include "../../include/structures/ArbolAVL.h"

#include <iostream>

// Constructor del árbol AVL
ArbolAVL::ArbolAVL()
{
    raiz = nullptr;
}

// Devuelve la altura de un nodo
int ArbolAVL::obtenerAltura(NodoAVL* nodo)
{
    if (nodo == nullptr)
    {
        return 0;
    }

    return nodo->altura;
}

// Devuelve el mayor entre dos números
int ArbolAVL::obtenerMayor(int a, int b)
{
    return (a > b) ? a : b;
}

// Calcula el factor de balance de un nodo
int ArbolAVL::obtenerBalance(NodoAVL* nodo)
{
    if (nodo == nullptr)
    {
        return 0;
    }

    return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
}

// Rotación simple a la derecha
NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* nodo)
{
    NodoAVL* nuevaRaiz = nodo->izquierdo;
    NodoAVL* subArbolTemporal = nuevaRaiz->derecho;

    nuevaRaiz->derecho = nodo;
    nodo->izquierdo = subArbolTemporal;

    nodo->altura = 1 + obtenerMayor(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));
    nuevaRaiz->altura = 1 + obtenerMayor(obtenerAltura(nuevaRaiz->izquierdo), obtenerAltura(nuevaRaiz->derecho));

    return nuevaRaiz;
}

// Rotación simple a la izquierda
NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* nodo)
{
    NodoAVL* nuevaRaiz = nodo->derecho;
    NodoAVL* subArbolTemporal = nuevaRaiz->izquierdo;

    nuevaRaiz->izquierdo = nodo;
    nodo->derecho = subArbolTemporal;

    nodo->altura = 1 + obtenerMayor(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));
    nuevaRaiz->altura = 1 + obtenerMayor(obtenerAltura(nuevaRaiz->izquierdo), obtenerAltura(nuevaRaiz->derecho));

    return nuevaRaiz;
}

// Inserta un producto en el árbol y mantiene el balance AVL
NodoAVL* ArbolAVL::insertar(NodoAVL* nodo, const Producto& producto)
{
    if (nodo == nullptr)
    {
        return new NodoAVL(producto);
    }

    if (producto.obtenerCodigo() < nodo->producto.obtenerCodigo())
    {
        nodo->izquierdo = insertar(nodo->izquierdo, producto);
    }
    else if (producto.obtenerCodigo() > nodo->producto.obtenerCodigo())
    {
        nodo->derecho = insertar(nodo->derecho, producto);
    }
    else
    {
        // No se permiten códigos duplicados
        return nodo;
    }

    nodo->altura = 1 + obtenerMayor(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));

    int balance = obtenerBalance(nodo);

    // Caso Izquierda-Izquierda
    if (balance > 1 && producto.obtenerCodigo() < nodo->izquierdo->producto.obtenerCodigo())
    {
        return rotarDerecha(nodo);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && producto.obtenerCodigo() > nodo->derecho->producto.obtenerCodigo())
    {
        return rotarIzquierda(nodo);
    }

    // Caso Izquierda-Derecha
    if (balance > 1 && producto.obtenerCodigo() > nodo->izquierdo->producto.obtenerCodigo())
    {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && producto.obtenerCodigo() < nodo->derecho->producto.obtenerCodigo())
    {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// Busca un nodo por código
NodoAVL* ArbolAVL::buscar(NodoAVL* nodo, int codigo)
{
    if (nodo == nullptr)
    {
        return nullptr;
    }

    if (codigo == nodo->producto.obtenerCodigo())
    {
        return nodo;
    }

    if (codigo < nodo->producto.obtenerCodigo())
    {
        return buscar(nodo->izquierdo, codigo);
    }

    return buscar(nodo->derecho, codigo);
}

// Recorre el árbol en orden e imprime los productos
void ArbolAVL::mostrarEnOrden(NodoAVL* nodo)
{
    if (nodo == nullptr)
    {
        return;
    }

    mostrarEnOrden(nodo->izquierdo);
    std::cout << nodo->producto.convertirATexto() << std::endl;
    mostrarEnOrden(nodo->derecho);
}

// Método público para insertar un producto
void ArbolAVL::insertarProducto(const Producto& producto)
{
    raiz = insertar(raiz, producto);
}

// Método público para buscar un producto por código
Producto* ArbolAVL::buscarProductoPorCodigo(int codigo)
{
    NodoAVL* nodoEncontrado = buscar(raiz, codigo);

    if (nodoEncontrado == nullptr)
    {
        return nullptr;
    }

    return &(nodoEncontrado->producto);
}

// Método público para mostrar todos los productos en orden
void ArbolAVL::mostrarEnOrden()
{
    mostrarEnOrden(raiz);
}

NodoAVL* ArbolAVL::obtenerNodoMinimo(NodoAVL* nodo)
{
    NodoAVL* actual = nodo;

    while (actual != nullptr && actual->izquierdo != nullptr)
    {
        actual = actual->izquierdo;
    }

    return actual;

}

NodoAVL* ArbolAVL::eliminar(NodoAVL* nodo, int codigo)
{
    if (nodo == nullptr)
    {
        return nodo;
    }

    //Buscar el nodo a eliminar
    if (codigo < nodo->producto.obtenerCodigo())
    {
        nodo->izquierdo = eliminar(nodo->izquierdo, codigo);
    }
    else if (codigo > nodo->producto.obtenerCodigo())
    {
        nodo->derecho = eliminar(nodo->derecho, codigo);
    }
    else
    {
        //Caso 1 y 2: tiene cero o un hijo
        if (nodo->izquierdo == nullptr || nodo->derecho == nullptr)
        {
            NodoAVL* temp = nullptr;

            if (nodo->izquierdo != nullptr) 
            {
                temp = nodo->izquierdo;
            }
            else
            {
                temp = nodo->derecho;
            }

            //Caso sin hijos

            if (temp == nullptr)
            {
                temp = nodo;
                nodo = nullptr;
            }
            else
            {
                *nodo = *temp;
            }
            delete temp;
        }
        else
        {
            NodoAVL* temp = obtenerNodoMinimo(nodo->derecho);

            nodo->producto = temp->producto;

            nodo->derecho = eliminar(nodo->derecho, temp->producto.obtenerCodigo());
        }
    }

    // Si el árbol quedó vacío
    if (nodo == nullptr)
    {
        return nodo;
    }

    // Actualizar altura
    nodo->altura = 1 + obtenerMayor(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));

    // Obtener balance
    int balance = obtenerBalance(nodo);

    // Caso Izquierda-Izquierda
    if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0)
    {
        return rotarDerecha(nodo);
    }

    // Caso Izquierda-Derecha
    if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0)
    {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && obtenerBalance(nodo->derecho) <= 0)
    {
        return rotarIzquierda(nodo);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && obtenerBalance(nodo->derecho) > 0)
    {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;

}

void ArbolAVL::eliminarProductoPorCodigo(int codigo)
{
    raiz = eliminar(raiz, codigo);
}