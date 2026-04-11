import { useEffect, useMemo, useState } from "react";
import { obtenerClientes } from "../services/clientesApi";
import { obtenerProductos } from "../services/productosApi";
import {
    obtenerPedidos,
    obtenerSiguientePedido,
    crearPedido,
    atenderPedido,
} from "../services/pedidosApi";

const lineaInicial = {
    productoCodigo: "",
    cantidad: 1,
};

function PedidosPage() {
    const [pedidos, setPedidos] = useState([]);
    const [clientes, setClientes] = useState([]);
    const [productos, setProductos] = useState([]);
    const [clienteId, setClienteId] = useState("");
    const [detalles, setDetalles] = useState([{ ...lineaInicial }]);
    const [siguientePedido, setSiguientePedido] = useState(null);
    const [pedidoAtendido, setPedidoAtendido] = useState(null);
    const [cargando, setCargando] = useState(true);
    const [guardando, setGuardando] = useState(false);
    const [error, setError] = useState("");
    const [mensaje, setMensaje] = useState("");

    useEffect(() => {
        cargarTodo();
    }, []);

    async function cargarTodo() {
        try {
            setCargando(true);
            setError("");

            const [pedidosData, clientesData, productosData] = await Promise.all([
                obtenerPedidos(),
                obtenerClientes(),
                obtenerProductos(),
            ]);

            setPedidos(pedidosData);
            setClientes(clientesData);
            setProductos(productosData.filter((p) => p.estado === "Activo"));

            try {
                const siguiente = await obtenerSiguientePedido();
                setSiguientePedido(siguiente);
            } catch {
                setSiguientePedido(null);
            }
        } catch (err) {
            setError(err.message);
        } finally {
            setCargando(false);
        }
    }

    function actualizarLinea(index, campo, valor) {
        const copia = [...detalles];
        copia[index][campo] = valor;
        setDetalles(copia);
    }

    function agregarLinea() {
        setDetalles([...detalles, { ...lineaInicial }]);
    }

    function eliminarLinea(index) {
        if (detalles.length === 1) return;
        setDetalles(detalles.filter((_, i) => i !== index));
    }

    const resumen = useMemo(() => {
        let total = 0;

        for (const linea of detalles) {
            const producto = productos.find(
                (p) => Number(p.codigo) === Number(linea.productoCodigo)
            );

            if (producto && Number(linea.cantidad) > 0) {
                total += Number(producto.precio) * Number(linea.cantidad);
            }
        }

        return { total };
    }, [detalles, productos]);

    async function manejarCrearPedido(e) {
        e.preventDefault();

        try {
            setGuardando(true);
            setError("");
            setMensaje("");
            setPedidoAtendido(null);

            const pedido = {
                clienteId: Number(clienteId),
                detalles: detalles.map((d) => ({
                    productoCodigo: Number(d.productoCodigo),
                    cantidad: Number(d.cantidad),
                })),
            };

            const resultado = await crearPedido(pedido);

            setMensaje(
                `Pedido pendiente creado correctamente. Número: ${resultado.numero ?? ""}`
            );
            setClienteId("");
            setDetalles([{ ...lineaInicial }]);

            await cargarTodo();
        } catch (err) {
            setError(err.message);
        } finally {
            setGuardando(false);
        }
    }

    async function manejarAtenderPedido() {
        try {
            setError("");
            setMensaje("");

            const resultado = await atenderPedido();
            setPedidoAtendido(resultado);
            setMensaje("Pedido atendido correctamente.");
            await cargarTodo();
        } catch (err) {
            setError(err.message);
        }
    }

    function nombreClientePorId(id) {
        const cliente = clientes.find((c) => Number(c.id) === Number(id));
        return cliente ? cliente.nombre : `Cliente ${id}`;
    }

    function nombreProductoPorCodigo(codigo) {
        const producto = productos.find((p) => Number(p.codigo) === Number(codigo));
        return producto ? producto.nombre : `Código ${codigo}`;
    }

    return (
        <div style={contenedor}>
            <div style={encabezado}>
                <div>
                    <h1 style={{ margin: 0 }}>Pedidos pendientes</h1>
                    <p style={textoSecundario}>
                        Gestiona la cola de pedidos antes de convertirlos en venta
                    </p>
                </div>

                <button style={botonSecundario} onClick={cargarTodo}>
                    Recargar
                </button>
            </div>

            {error && <p style={textoError}>Error: {error}</p>}
            {mensaje && <p style={textoExito}>{mensaje}</p>}

            <div style={gridPrincipal}>
                <div style={columnaIzquierda}>
                    <div style={card}>
                        <h2 style={subtitulo}>Nuevo pedido</h2>

                        <form onSubmit={manejarCrearPedido} style={formularioEstilo}>
                            <label style={label}>Cliente</label>
                            <select
                                style={input}
                                value={clienteId}
                                onChange={(e) => setClienteId(e.target.value)}
                                required
                            >
                                <option value="">Seleccione un cliente</option>
                                {clientes.map((cliente) => (
                                    <option key={cliente.id} value={cliente.id}>
                                        {cliente.nombre} ({cliente.id})
                                    </option>
                                ))}
                            </select>

                            <label style={label}>Productos</label>

                            {detalles.map((linea, index) => (
                                <div key={index} style={lineaCard}>
                                    <select
                                        style={input}
                                        value={linea.productoCodigo}
                                        onChange={(e) =>
                                            actualizarLinea(index, "productoCodigo", e.target.value)
                                        }
                                        required
                                    >
                                        <option value="">Seleccione un producto</option>
                                        {productos.map((producto) => (
                                            <option key={producto.codigo} value={producto.codigo}>
                                                {producto.nombre} - ₡{producto.precio}
                                            </option>
                                        ))}
                                    </select>

                                    <input
                                        style={input}
                                        type="number"
                                        min="1"
                                        value={linea.cantidad}
                                        onChange={(e) =>
                                            actualizarLinea(index, "cantidad", e.target.value)
                                        }
                                        required
                                    />

                                    <button
                                        type="button"
                                        style={botonEliminar}
                                        onClick={() => eliminarLinea(index)}
                                    >
                                        Quitar
                                    </button>
                                </div>
                            ))}

                            <button type="button" style={botonNeutro} onClick={agregarLinea}>
                                Agregar línea
                            </button>

                            <div style={resumenCard}>
                                <p style={resumenTotal}>
                                    <span>Total estimado:</span>
                                    <strong>₡{resumen.total.toFixed(2)}</strong>
                                </p>
                            </div>

                            <button style={botonPrimario} type="submit" disabled={guardando}>
                                {guardando ? "Guardando..." : "Crear pedido"}
                            </button>
                        </form>
                    </div>

                    <div style={card}>
                        <div style={cabeceraListado}>
                            <h2 style={subtitulo}>Cola de pedidos</h2>
                            <span style={contadorBadge}>{pedidos.length} en cola</span>
                        </div>

                        {cargando ? (
                            <p>Cargando pedidos...</p>
                        ) : pedidos.length === 0 ? (
                            <p>No hay pedidos pendientes.</p>
                        ) : (
                            <div style={listaPedidos}>
                                {pedidos.map((pedido) => (
                                    <div key={pedido.numero} style={pedidoItem}>
                                        <div>
                                            <p style={pedidoTitulo}>Pedido #{pedido.numero}</p>
                                            <p style={pedidoTexto}>
                                                Cliente: {nombreClientePorId(pedido.clienteId)}
                                            </p>
                                            <p style={pedidoTexto}>Estado: {pedido.estado}</p>
                                            <p style={pedidoTexto}>
                                                Total: ₡{Number(pedido.total || 0).toFixed(2)}
                                            </p>
                                        </div>
                                    </div>
                                ))}
                            </div>
                        )}
                    </div>
                </div>

                <div style={columnaDerecha}>
                    <div style={card}>
                        <h2 style={subtitulo}>Siguiente pedido</h2>

                        {siguientePedido ? (
                            <div>
                                <p><strong>Número:</strong> {siguientePedido.numero}</p>
                                <p>
                                    <strong>Cliente:</strong>{" "}
                                    {nombreClientePorId(siguientePedido.clienteId)}
                                </p>
                                <p><strong>Estado:</strong> {siguientePedido.estado}</p>
                                <p>
                                    <strong>Total:</strong> ₡
                                    {Number(siguientePedido.total || 0).toFixed(2)}
                                </p>

                                {siguientePedido.detalles?.length > 0 && (
                                    <>
                                        <h3>Detalle</h3>
                                        {siguientePedido.detalles.map((d, i) => (
                                            <div key={i} style={detalleLinea}>
                                                <p>Producto: {nombreProductoPorCodigo(d.productoCodigo)}</p>
                                                <p>Cantidad: {d.cantidad}</p>
                                            </div>
                                        ))}
                                    </>
                                )}

                                <button style={botonPrimario} onClick={manejarAtenderPedido}>
                                    Atender pedido
                                </button>
                            </div>
                        ) : (
                            <p>No hay pedidos pendientes en la cola.</p>
                        )}
                    </div>

                    <div style={card}>
                        <h2 style={subtitulo}>Último pedido atendido</h2>

                        {pedidoAtendido ? (
                            <div>
                                <p><strong>Número:</strong> {pedidoAtendido.numero}</p>
                                <p>
                                    <strong>Cliente:</strong>{" "}
                                    {nombreClientePorId(pedidoAtendido.clienteId)}
                                </p>
                                <p><strong>Estado:</strong> {pedidoAtendido.estado}</p>
                                <p>
                                    <strong>Total:</strong> ₡
                                    {Number(pedidoAtendido.total || 0).toFixed(2)}
                                </p>
                            </div>
                        ) : (
                            <p>No has atendido pedidos en esta sesión.</p>
                        )}
                    </div>
                </div>
            </div>
        </div>
    );
}

const contenedor = {
    padding: "24px",
    backgroundColor: "#f4f6f8",
    minHeight: "100vh",
};

const encabezado = {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    marginBottom: "24px",
};

const gridPrincipal = {
    display: "grid",
    gridTemplateColumns: "1.2fr 0.9fr",
    gap: "24px",
    alignItems: "start",
};

const columnaIzquierda = {
    display: "flex",
    flexDirection: "column",
    gap: "24px",
};

const columnaDerecha = {
    display: "flex",
    flexDirection: "column",
    gap: "24px",
};

const card = {
    backgroundColor: "white",
    borderRadius: "14px",
    padding: "20px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.08)",
    border: "1px solid #e5e7eb",
};

const subtitulo = {
    marginTop: 0,
    marginBottom: "16px",
};

const textoSecundario = {
    margin: 0,
    color: "#6b7280",
    fontSize: "14px",
};

const formularioEstilo = {
    display: "flex",
    flexDirection: "column",
    gap: "12px",
};

const label = {
    fontWeight: "bold",
    fontSize: "14px",
};

const input = {
    padding: "10px 12px",
    border: "1px solid #d1d5db",
    borderRadius: "8px",
    fontSize: "14px",
    width: "100%",
};

const lineaCard = {
    border: "1px solid #e5e7eb",
    borderRadius: "10px",
    padding: "12px",
    display: "grid",
    gridTemplateColumns: "1fr 120px 100px",
    gap: "10px",
    backgroundColor: "#f9fafb",
    alignItems: "center",
};

const resumenCard = {
    border: "1px solid #d1d5db",
    borderRadius: "10px",
    padding: "14px",
    backgroundColor: "#f9fafb",
};

const resumenTotal = {
    display: "flex",
    justifyContent: "space-between",
    margin: 0,
    fontSize: "18px",
};

const botonPrimario = {
    backgroundColor: "#111827",
    color: "white",
    border: "none",
    padding: "12px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

const botonSecundario = {
    backgroundColor: "#2563eb",
    color: "white",
    border: "none",
    padding: "10px 14px",
    borderRadius: "8px",
    cursor: "pointer",
};

const botonNeutro = {
    backgroundColor: "#374151",
    color: "white",
    border: "none",
    padding: "10px 12px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

const botonEliminar = {
    backgroundColor: "#dc2626",
    color: "white",
    border: "none",
    padding: "10px 10px",
    borderRadius: "6px",
    cursor: "pointer",
};

const textoError = {
    color: "#dc2626",
    fontWeight: "bold",
};

const textoExito = {
    color: "#16a34a",
    fontWeight: "bold",
};

const cabeceraListado = {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    marginBottom: "12px",
};

const contadorBadge = {
    backgroundColor: "#dbeafe",
    color: "#1d4ed8",
    padding: "6px 10px",
    borderRadius: "999px",
    fontSize: "12px",
    fontWeight: "bold",
};

const listaPedidos = {
    display: "flex",
    flexDirection: "column",
    gap: "12px",
};

const pedidoItem = {
    border: "1px solid #e5e7eb",
    borderRadius: "10px",
    padding: "14px",
    backgroundColor: "#f9fafb",
};

const pedidoTitulo = {
    margin: "0 0 6px 0",
    fontWeight: "bold",
};

const pedidoTexto = {
    margin: "2px 0",
    color: "#4b5563",
    fontSize: "14px",
};

const detalleLinea = {
    borderTop: "1px solid #e5e7eb",
    marginTop: "10px",
    paddingTop: "10px",
};

export default PedidosPage;