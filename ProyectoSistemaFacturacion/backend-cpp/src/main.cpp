#include <iostream>
#include <string>

#include "../include/services/InventarioService.h"
#include "../include/services/ClienteService.h"

using namespace std;

void mostrarMenuPrincipal()
{
    cout << endl;
    cout << "============= MENU PRINCIPAL =============" << endl;
    cout << "1. Gestionar productos" << endl;
    cout << "2. Gestionar clientes" << endl;
    cout << "3. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuProductos()
{
    cout << endl;
    cout << "========= MENU PRODUCTOS =========" << endl;
    cout << "1. Registrar producto" << endl;
    cout << "2. Buscar producto por codigo" << endl;
    cout << "3. Eliminar producto por codigo" << endl;
    cout << "4. Mostrar inventario" << endl;
    cout << "5. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuClientes()
{
    cout << endl;
    cout << "========= MENU CLIENTES =========" << endl;
    cout << "1. Registrar cliente" << endl;
    cout << "2. Buscar cliente por ID" << endl;
    cout << "3. Eliminar cliente por ID" << endl;
    cout << "4. Mostrar clientes" << endl;
    cout << "5. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
}

int main()
{
    InventarioService inventario;
    ClienteService clienteService;

    int cantidadProductos = inventario.cargarProductosIniciales();
    int cantidadClientes = clienteService.cargarClientesIniciales();

    cout << "==============================================" << endl;
    cout << "Sistema iniciado correctamente." << endl;
    cout << "Productos cargados desde API/Azure: " << cantidadProductos << endl;
    cout << "Clientes cargados desde API/Azure: " << cantidadClientes << endl;
    cout << "==============================================" << endl;

    int opcionPrincipal = 0;

    do
    {
        mostrarMenuPrincipal();
        cin >> opcionPrincipal;

        switch (opcionPrincipal)
        {
        case 1:
        {
            int opcionProductos = 0;

            do
            {
                mostrarMenuProductos();
                cin >> opcionProductos;

                switch (opcionProductos)
                {
                case 1:
                {
                    int codigo;
                    string nombre;
                    string categoria;
                    string marca;
                    double precio;
                    int stock;
                    string estado;

                    cout << "Codigo: ";
                    cin >> codigo;

                    cin.ignore();

                    cout << "Nombre: ";
                    getline(cin, nombre);

                    cout << "Categoria: ";
                    getline(cin, categoria);

                    cout << "Marca: ";
                    getline(cin, marca);

                    cout << "Precio: ";
                    cin >> precio;

                    cout << "Stock: ";
                    cin >> stock;

                    cin.ignore();

                    cout << "Estado: ";
                    getline(cin, estado);

                    Producto nuevoProducto(codigo, nombre, categoria, marca, precio, stock, estado);

                    if (inventario.registrarProducto(nuevoProducto))
                    {
                        cout << "Producto registrado correctamente en API y AVL." << endl;
                    }
                    else
                    {
                        cout << "No se pudo registrar el producto. Puede que el codigo ya exista o que la API no este disponible." << endl;
                    }

                    break;
                }

                case 2:
                {
                    int codigo;
                    cout << "Ingrese el codigo a buscar: ";
                    cin >> codigo;

                    Producto* encontrado = inventario.buscarProductoPorCodigo(codigo);

                    if (encontrado != nullptr)
                    {
                        cout << endl << "Producto encontrado:" << endl;
                        encontrado->mostrar();
                    }
                    else
                    {
                        cout << "Producto no encontrado." << endl;
                    }

                    break;
                }

                case 3:
                {
                    int codigo;
                    cout << "Ingrese el codigo a eliminar: ";
                    cin >> codigo;

                    if (inventario.eliminarProductoPorCodigo(codigo))
                    {
                        cout << "Producto eliminado correctamente de API y AVL." << endl;
                    }
                    else
                    {
                        cout << "No se pudo eliminar el producto. Puede que no exista o que la API no este disponible." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << endl << "=== INVENTARIO ACTUAL ===" << endl;
                    inventario.mostrarInventario();
                    break;
                }

                case 5:
                {
                    cout << "Volviendo al menu principal..." << endl;
                    break;
                }

                default:
                {
                    cout << "Opcion invalida." << endl;
                    break;
                }
                }

            } while (opcionProductos != 5);

            break;
        }

        case 2:
        {
            int opcionClientes = 0;

            do
            {
                mostrarMenuClientes();
                cin >> opcionClientes;

                switch (opcionClientes)
                {
                case 1:
                {
                    int id;
                    string nombre;
                    string correo;
                    string telefono;
                    string direccion;

                    cout << "ID: ";
                    cin >> id;

                    cin.ignore();

                    cout << "Nombre: ";
                    getline(cin, nombre);

                    cout << "Correo: ";
                    getline(cin, correo);

                    cout << "Telefono: ";
                    getline(cin, telefono);

                    cout << "Direccion: ";
                    getline(cin, direccion);

                    Cliente nuevoCliente(id, nombre, correo, telefono, direccion);

                    if (clienteService.registrarCliente(nuevoCliente))
                    {
                        cout << "Cliente registrado correctamente en API y memoria." << endl;
                    }
                    else
                    {
                        cout << "No se pudo registrar el cliente. Puede que el ID ya exista o que la API no este disponible." << endl;
                    }

                    break;
                }

                case 2:
                {
                    int id;
                    cout << "Ingrese el ID a buscar: ";
                    cin >> id;

                    Cliente* encontrado = clienteService.buscarClientePorId(id);

                    if (encontrado != nullptr)
                    {
                        cout << endl << "Cliente encontrado:" << endl;
                        encontrado->mostrar();
                    }
                    else
                    {
                        cout << "Cliente no encontrado." << endl;
                    }

                    break;
                }

                case 3:
                {
                    int id;
                    cout << "Ingrese el ID a eliminar: ";
                    cin >> id;

                    if (clienteService.eliminarClientePorId(id))
                    {
                        cout << "Cliente eliminado correctamente de API y memoria." << endl;
                    }
                    else
                    {
                        cout << "No se pudo eliminar el cliente. Puede que no exista o que la API no este disponible." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << endl << "=== CLIENTES ACTUALES ===" << endl;
                    clienteService.mostrarClientes();
                    break;
                }

                case 5:
                {
                    cout << "Volviendo al menu principal..." << endl;
                    break;
                }

                default:
                {
                    cout << "Opcion invalida." << endl;
                    break;
                }
                }

            } while (opcionClientes != 5);

            break;
        }

        case 3:
        {
            cout << "Saliendo del sistema..." << endl;
            break;
        }

        default:
        {
            cout << "Opcion invalida." << endl;
            break;
        }
        }

    } while (opcionPrincipal != 3);

    return 0;
}