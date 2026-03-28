#include "../../include/structures/ArbolBMasVentas.h"

#include <algorithm>
#include <iostream>

ArbolBMasVentas::ArbolBMasVentas(int grado)
    : raiz(nullptr), grado(grado)
{
}

ArbolBMasVentas::~ArbolBMasVentas()
{
    liberarMemoria(raiz);
}

NodoBMas* ArbolBMasVentas::buscarHoja(int clave)
{
    if (raiz == nullptr)
    {
        return nullptr;
    }

    NodoBMas* actual = raiz;

    while (!actual->esHoja)
    {
        size_t i = 0;

        while (i < actual->claves.size() && clave >= actual->claves[i])
        {
            i++;
        }

        actual = actual->hijos[i];
    }

    return actual;
}

void ArbolBMasVentas::insertarEnHoja(NodoBMas* hoja, const Venta& venta)
{
    int clave = venta.getNumero();

    auto itClave = hoja->claves.begin();
    auto itValor = hoja->valores.begin();

    while (itClave != hoja->claves.end() && *itClave < clave)
    {
        ++itClave;
        ++itValor;
    }

    hoja->claves.insert(itClave, clave);
    hoja->valores.insert(itValor, venta);
}

void ArbolBMasVentas::dividirHoja(NodoBMas* hoja)
{
    int puntoMedio = static_cast<int>((hoja->claves.size() + 1) / 2);

    NodoBMas* nuevaHoja = new NodoBMas(true);

    nuevaHoja->claves.assign(
        hoja->claves.begin() + puntoMedio,
        hoja->claves.end()
    );

    nuevaHoja->valores.assign(
        hoja->valores.begin() + puntoMedio,
        hoja->valores.end()
    );

    hoja->claves.erase(hoja->claves.begin() + puntoMedio, hoja->claves.end());
    hoja->valores.erase(hoja->valores.begin() + puntoMedio, hoja->valores.end());

    nuevaHoja->siguiente = hoja->siguiente;
    hoja->siguiente = nuevaHoja;

    int claveSubir = nuevaHoja->claves.front();

    if (hoja == raiz)
    {
        NodoBMas* nuevaRaiz = new NodoBMas(false);
        nuevaRaiz->claves.push_back(claveSubir);
        nuevaRaiz->hijos.push_back(hoja);
        nuevaRaiz->hijos.push_back(nuevaHoja);
        raiz = nuevaRaiz;
    }
    else
    {
        insertarEnPadre(hoja, claveSubir, nuevaHoja);
    }
}

void ArbolBMasVentas::insertarEnPadre(NodoBMas* izquierda, int clave, NodoBMas* derecha)
{
    if (izquierda == raiz)
    {
        NodoBMas* nuevaRaiz = new NodoBMas(false);
        nuevaRaiz->claves.push_back(clave);
        nuevaRaiz->hijos.push_back(izquierda);
        nuevaRaiz->hijos.push_back(derecha);
        raiz = nuevaRaiz;
        return;
    }

    NodoBMas* padre = nullptr;
    std::vector<NodoBMas*> pila;
    pila.push_back(raiz);

    while (!pila.empty())
    {
        NodoBMas* actual = pila.back();
        pila.pop_back();

        if (!actual->esHoja)
        {
            for (NodoBMas* hijo : actual->hijos)
            {
                if (hijo == izquierda)
                {
                    padre = actual;
                    break;
                }
            }

            if (padre != nullptr)
            {
                break;
            }

            for (NodoBMas* hijo : actual->hijos)
            {
                pila.push_back(hijo);
            }
        }
    }

    if (padre == nullptr)
    {
        return;
    }

    auto itHijo = std::find(padre->hijos.begin(), padre->hijos.end(), izquierda);
    size_t indice = static_cast<size_t>(itHijo - padre->hijos.begin());

    padre->claves.insert(padre->claves.begin() + indice, clave);
    padre->hijos.insert(padre->hijos.begin() + indice + 1, derecha);

    if (static_cast<int>(padre->claves.size()) >= grado)
    {
        dividirInterno(padre);
    }
}

void ArbolBMasVentas::dividirInterno(NodoBMas* nodoInterno)
{
    int puntoMedio = static_cast<int>(nodoInterno->claves.size() / 2);
    int claveSubir = nodoInterno->claves[puntoMedio];

    NodoBMas* nuevoInterno = new NodoBMas(false);

    nuevoInterno->claves.assign(
        nodoInterno->claves.begin() + puntoMedio + 1,
        nodoInterno->claves.end()
    );

    nuevoInterno->hijos.assign(
        nodoInterno->hijos.begin() + puntoMedio + 1,
        nodoInterno->hijos.end()
    );

    nodoInterno->claves.erase(
        nodoInterno->claves.begin() + puntoMedio,
        nodoInterno->claves.end()
    );

    nodoInterno->hijos.erase(
        nodoInterno->hijos.begin() + puntoMedio + 1,
        nodoInterno->hijos.end()
    );

    if (nodoInterno == raiz)
    {
        NodoBMas* nuevaRaiz = new NodoBMas(false);
        nuevaRaiz->claves.push_back(claveSubir);
        nuevaRaiz->hijos.push_back(nodoInterno);
        nuevaRaiz->hijos.push_back(nuevoInterno);
        raiz = nuevaRaiz;
    }
    else
    {
        insertarEnPadre(nodoInterno, claveSubir, nuevoInterno);
    }
}

void ArbolBMasVentas::insertarVenta(const Venta& venta)
{
    if (raiz == nullptr)
    {
        raiz = new NodoBMas(true);
        raiz->claves.push_back(venta.getNumero());
        raiz->valores.push_back(venta);
        return;
    }

    NodoBMas* hoja = buscarHoja(venta.getNumero());

    for (int claveExistente : hoja->claves)
    {
        if (claveExistente == venta.getNumero())
        {
            return;
        }
    }

    insertarEnHoja(hoja, venta);

    if (static_cast<int>(hoja->claves.size()) >= grado)
    {
        dividirHoja(hoja);
    }
}

Venta* ArbolBMasVentas::buscarVentaPorNumero(int numero)
{
    NodoBMas* hoja = buscarHoja(numero);

    if (hoja == nullptr)
    {
        return nullptr;
    }

    for (size_t i = 0; i < hoja->claves.size(); i++)
    {
        if (hoja->claves[i] == numero)
        {
            return &hoja->valores[i];
        }
    }

    return nullptr;
}

void ArbolBMasVentas::mostrarEnOrdenDesdeHoja(NodoBMas* hoja) const
{
    NodoBMas* actual = hoja;

    while (actual != nullptr)
    {
        for (const Venta& venta : actual->valores)
        {
            venta.mostrar();
            std::cout << "-----------------------------" << std::endl;
        }

        actual = actual->siguiente;
    }
}

void ArbolBMasVentas::mostrarVentasEnOrden() const
{
    if (raiz == nullptr)
    {
        std::cout << "No hay ventas registradas en el Arbol B+." << std::endl;
        return;
    }

    NodoBMas* actual = raiz;

    while (actual != nullptr && !actual->esHoja)
    {
        actual = actual->hijos.front();
    }

    mostrarEnOrdenDesdeHoja(actual);
}

void ArbolBMasVentas::liberarMemoria(NodoBMas* nodo)
{
    if (nodo == nullptr)
    {
        return;
    }

    if (!nodo->esHoja)
    {
        for (NodoBMas* hijo : nodo->hijos)
        {
            liberarMemoria(hijo);
        }
    }

    delete nodo;
}