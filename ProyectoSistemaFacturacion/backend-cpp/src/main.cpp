#include <iostream>
#include <string>
#include <vector>

#include "../include/services/InventarioService.h"
#include "../include/services/ClienteService.h"
#include "../include/services/VentaService.h"
#include "../include/services/PedidoService.h"

using namespace std;

void mostrarMenuPrincipal()
{
    cout << endl;
    cout << "============= MENU PRINCIPAL =============" << endl;
    cout << "1. Gestionar productos" << endl;
    cout << "2. Gestionar clientes" << endl;
    cout << "3. Gestionar ventas" << endl;
    cout << "4. Gestionar pedidos pendientes" << endl;
    cout << "5. Salir" << endl;
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

void mostrarMenuVentas()
{
    cout << endl;
    cout << "========= MENU VENTAS =========" << endl;
    cout << "1. Registrar venta" << endl;
    cout << "2. Buscar venta por numero" << endl;
    cout << "3. Mostrar ventas" << endl;
    cout << "4. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuPedidos()
{
    cout << endl;
    cout << "========= MENU PEDIDOS PENDIENTES =========" << endl;
    cout << "1. Crear y encolar pedido pendiente" << endl;
    cout << "2. Ver siguiente pedido" << endl;
    cout << "3. Atender siguiente pedido" << endl;
    cout << "4. Mostrar cola de pedidos" << endl;
    cout << "5. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
}

int main()
{
    InventarioService inventario;
    ClienteService clienteService;
    VentaService ventaService;
    PedidoService pedidoService;

    int cantidadProductos = inventario.cargarProductosIniciales();
    int cantidadClientes = clienteService.cargarClientesIniciales();
    ventaService.cargarVentasIniciales();

    cout << "==============================================" << endl;
    cout << "Sistema iniciado correctamente." << endl;
    cout << "Productos cargados desde API/Azure: " << cantidadProductos << endl;
    cout << "Clientes cargados desde API/Azure: " << cantidadClientes << endl;
    cout << "Ventas cargadas desde API/Azure." << endl;
    cout << "Cola de pedidos lista en memoria." << endl;
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
            int opcionVentas = 0;

            do
            {
                mostrarMenuVentas();
                cin >> opcionVentas;

                switch (opcionVentas)
                {
                case 1:
                {
                    int clienteId;
                    int cantidadDetalles;

                    cout << "Ingrese el ID del cliente: ";
                    cin >> clienteId;

                    cout << "Cuantos productos desea agregar a la venta: ";
                    cin >> cantidadDetalles;

                    vector<DetalleVenta> detalles;

                    for (int i = 0; i < cantidadDetalles; i++)
                    {
                        int productoCodigo;
                        int cantidad;

                        cout << "Producto #" << (i + 1) << endl;
                        cout << "Codigo del producto: ";
                        cin >> productoCodigo;
                        cout << "Cantidad: ";
                        cin >> cantidad;

                        DetalleVenta detalle;
                        detalle.setProductoCodigo(productoCodigo);
                        detalle.setCantidad(cantidad);

                        detalles.push_back(detalle);
                    }

                    Venta ventaRegistrada;

                    if (ventaService.registrarVenta(clienteId, detalles, ventaRegistrada))
                    {
                        cout << "Venta registrada correctamente." << endl;
                        cout << "Numero generado: " << ventaRegistrada.getNumero() << endl;
                        cout << "Total: " << ventaRegistrada.getTotal() << endl;
                    }
                    else
                    {
                        cout << "No se pudo registrar la venta. Revise cliente, productos, stock o disponibilidad de la API." << endl;
                    }

                    break;
                }

                case 2:
                {
                    int numero;
                    cout << "Ingrese el numero de la venta: ";
                    cin >> numero;

                    Venta* encontrada = ventaService.buscarVentaPorNumero(numero);

                    if (encontrada != nullptr)
                    {
                        cout << endl << "Venta encontrada:" << endl;
                        encontrada->mostrar();
                    }
                    else
                    {
                        cout << "Venta no encontrada." << endl;
                    }

                    break;
                }

                case 3:
                {
                    cout << endl << "=== VENTAS ACTUALES ===" << endl;
                    ventaService.mostrarVentas();
                    break;
                }

                case 4:
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

            } while (opcionVentas != 4);

            break;
        }

        case 4:
        {
            int opcionPedidos = 0;

            do
            {
                mostrarMenuPedidos();
                cin >> opcionPedidos;

                switch (opcionPedidos)
                {
                case 1:
                {
                    int clienteId;
                    string fecha;
                    double total;

                    cout << "Ingrese el ID del cliente: ";
                    cin >> clienteId;

                    cin.ignore();

                    cout << "Ingrese la fecha del pedido: ";
                    getline(cin, fecha);

                    cout << "Ingrese el total del pedido: ";
                    cin >> total;

                    PedidoPendiente pedidoCreado = pedidoService.crearYEncolarPedido(clienteId, fecha, total);

                    cout << "Pedido pendiente creado y encolado correctamente." << endl;
                    cout << "Numero generado: " << pedidoCreado.getNumero() << endl;

                    break;
                }

                case 2:
                {
                    PedidoPendiente siguientePedido;

                    if (pedidoService.verSiguientePedido(siguientePedido))
                    {
                        cout << endl << "=== SIGUIENTE PEDIDO EN COLA ===" << endl;
                        siguientePedido.mostrar();
                    }
                    else
                    {
                        cout << "No hay pedidos pendientes en la cola." << endl;
                    }

                    break;
                }

                case 3:
                {
                    PedidoPendiente pedidoAtendido;

                    if (pedidoService.atenderSiguientePedido(pedidoAtendido))
                    {
                        cout << "Se atendio el siguiente pedido correctamente:" << endl;
                        pedidoAtendido.mostrar();
                    }
                    else
                    {
                        cout << "No hay pedidos pendientes para atender." << endl;
                    }

                    break;
                }

                case 4:
                {
                    pedidoService.mostrarPedidosPendientes();
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

            } while (opcionPedidos != 5);

            break;
        }

        case 5:
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

    } while (opcionPrincipal != 5);

    return 0;
}