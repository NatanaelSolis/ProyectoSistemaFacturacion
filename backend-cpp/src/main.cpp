#include "../include/structures/ArbolAVL.h"

#include <iostream>

int main()
{
    ArbolAVL inventario;

    // Crear productos de prueba
    Producto producto1(103, "Mouse", 8500.0, 15);
    Producto producto2(101, "Teclado", 12500.0, 10);
    Producto producto3(105, "Monitor", 89500.0, 5);
    Producto producto4(102, "Audifonos", 18900.0, 20);
    Producto producto5(104, "Laptop", 450000.0, 3);

    // Insertar productos en el árbol AVL
    inventario.insertarProducto(producto1);
    inventario.insertarProducto(producto2);
    inventario.insertarProducto(producto3);
    inventario.insertarProducto(producto4);
    inventario.insertarProducto(producto5);

    std::cout << "========================================" << std::endl;
    std::cout << "PRODUCTOS REGISTRADOS EN ORDEN" << std::endl;
    std::cout << "========================================" << std::endl;
    inventario.mostrarEnOrden();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "BUSQUEDA DE PRODUCTO" << std::endl;
    std::cout << "========================================" << std::endl;

    int codigoBuscado = 102;
    Producto* productoEncontrado = inventario.buscarProductoPorCodigo(codigoBuscado);

    if (productoEncontrado != nullptr)
    {
        std::cout << "Producto encontrado:" << std::endl;
        std::cout << productoEncontrado->convertirATexto() << std::endl;
    }
    else
    {
        std::cout << "No se encontro un producto con el codigo " << codigoBuscado << std::endl;
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "BUSQUEDA DE PRODUCTO NO EXISTENTE" << std::endl;
    std::cout << "========================================" << std::endl;

    codigoBuscado = 999;
    productoEncontrado = inventario.buscarProductoPorCodigo(codigoBuscado);

    if (productoEncontrado != nullptr)
    {
        std::cout << "Producto encontrado:" << std::endl;
        std::cout << productoEncontrado->convertirATexto() << std::endl;
    }
    else
    {
        std::cout << "No se encontro un producto con el codigo " << codigoBuscado << std::endl;
    }

    return 0;
}