import { useEffect, useMemo, useState } from "react";
import { obtenerClientes } from "../services/clientesApi";
import { obtenerProductos } from "../services/productosApi";
import {
    crearVenta,
    obtenerVentaPorNumero,
    obtenerVentas,
} from "../services/ventasApi";
import FacturaPrintModal from "../components/FacturaPrintModal";

const lineaInicial = {
    productoCodigo: "",
    cantidad: 1,
    precioUnitario: "",
};

function VentasPage() {
    const [ventas, setVentas] = useState([]);
    const [clientes, setClientes] = useState([]);
    const [productos, setProductos] = useState([]);
    const [clienteId, setClienteId] = useState("");
    const [detalles, setDetalles] = useState([{ ...lineaInicial }]);
    const [numeroBusqueda, setNumeroBusqueda] = useState("");
    const [ventaSeleccionada, setVentaSeleccionada] = useState(null);
    const [modalFacturaAbierto, setModalFacturaAbierto] = useState(false);
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

            const [ventasData, clientesData, productosData] = await Promise.all([
                obtenerVentas(),
                obtenerClientes(),
                obtenerProductos(),
            ]);

            setVentas(ventasData);
            setClientes(clientesData);
            setProductos(productosData);
        } catch (err) {
            setError(err.message);
        } finally {
            setCargando(false);
        }
    }

    const productosActivos = productos.filter((p) => p.estado === "Activo");

    function actualizarLinea(index, campo, valor) {
        const copia = [...detalles];
        copia[index][campo] = valor;

        if (campo === "productoCodigo") {
            const producto = productosActivos.find(
                (p) => Number(p.codigo) === Number(valor)
            );

            if (producto) {
                copia[index].precioUnitario = String(producto.precio);
            }
        }

        setDetalles(copia);
    }

    function agregarLinea() {
        setDetalles([...detalles, { ...lineaInicial }]);
    }

    function eliminarLinea(index) {
        if (detalles.length === 1) return;
        setDetalles(detalles.filter((_, i) => i !== index));
    }

    function subtotalLinea(linea) {
        const cantidad = Number(linea.cantidad || 0);
        const precioUnitario = Number(linea.precioUnitario || 0);
        return cantidad * precioUnitario;
    }

    const resumen = useMemo(() => {
        let total = 0;

        for (const linea of detalles) {
            total += subtotalLinea(linea);
        }

        const subtotal = total / 1.13;
        const IVA = total - subtotal;

        return {
            subtotal,
            IVA,
            total,
        };
    }, [detalles]);

    async function manejarCrearVenta(e) {
        e.preventDefault();

        try {
            setGuardando(true);
            setError("");
            setMensaje("");

            const venta = {
                clienteId: Number(clienteId),
                detalles: detalles.map((d) => ({
                    productoCodigo: Number(d.productoCodigo),
                    cantidad: Number(d.cantidad),
                    precioUnitario: Number(d.precioUnitario),
                })),
            };

            const resultado = await crearVenta(venta);
            const ventaCompleta = await obtenerVentaPorNumero(resultado.numero);

            setMensaje(`Venta registrada correctamente. Factura #${resultado.numero}`);
            setClienteId("");
            setDetalles([{ ...lineaInicial }]);
            setVentaSeleccionada(ventaCompleta);
            setModalFacturaAbierto(true);
            await cargarTodo();
        } catch (err) {
            setError(err.message);
        } finally {
            setGuardando(false);
        }
    }

    async function manejarBuscarVenta() {
        try {
            setError("");
            setMensaje("");

            if (!numeroBusqueda) {
                setVentaSeleccionada(null);
                return;
            }

            const venta = await obtenerVentaPorNumero(Number(numeroBusqueda));
            setVentaSeleccionada(venta);
            setModalFacturaAbierto(true);
        } catch (err) {
            setVentaSeleccionada(null);
            setError(err.message);
        }
    }

    async function abrirFactura(ventaBase) {
        try {
            setError("");
            const ventaCompleta = await obtenerVentaPorNumero(Number(ventaBase.numero));
            setVentaSeleccionada(ventaCompleta);
            setModalFacturaAbierto(true);
        } catch (err) {
            setError(err.message);
        }
    }

    function imprimirFactura() {
        const contenido = document.getElementById("factura-print-area");
        if (!contenido) return;

        const ventana = window.open("", "_blank", "width=1000,height=800");
        if (!ventana) return;

        ventana.document.write(`
      <html>
        <head>
          <title>Factura</title>
          <style>
            body {
              font-family: Arial, sans-serif;
              margin: 0;
              padding: 24px;
              background: white;
              color: #111827;
            }
            table {
              width: 100%;
              border-collapse: collapse;
              margin-top: 12px;
            }
            th {
              background: #111827;
              color: white;
              text-align: left;
              padding: 12px;
              font-size: 14px;
            }
            td {
              border-bottom: 1px solid #e5e7eb;
              padding: 12px;
              font-size: 14px;
            }
          </style>
        </head>
        <body>
          ${contenido.innerHTML}
        </body>
      </html>
    `);

        ventana.document.close();
        ventana.focus();
        ventana.print();
        ventana.close();
    }

    function nombreClientePorId(id) {
        const cliente = clientes.find((c) => Number(c.id) === Number(id));
        return cliente ? cliente.nombre : `Cliente ${id}`;
    }

    function nombreProductoPorCodigo(codigo) {
        const producto = productos.find((p) => Number(p.codigo) === Number(codigo));
        return producto ? producto.nombre : `Código ${codigo}`;
    }

    const detallesConNombre = (ventaSeleccionada?.detalles || []).map((d) => ({
        ...d,
        nombreProducto: nombreProductoPorCodigo(d.productoCodigo),
    }));

    const ventasOrdenadas = [...ventas].sort((a, b) => Number(b.numero) - Number(a.numero));

    return (
        <div style={contenedor}>
            <div style={encabezado}>
                <div>
                    <h1 style={{ margin: 0 }}>Punto de Venta</h1>
                    <p style={textoSecundario}>
                        Registra ventas con precio editable por línea
                    </p>
                </div>

                <button style={botonSecundario} onClick={cargarTodo}>
                    Recargar
                </button>
            </div>

            {error && <p style={textoError}>Error: {error}</p>}
            {mensaje && <p style={textoExito}>{mensaje}</p>}

            <div style={gridPrincipal}>
                <div style={card}>
                    <h2 style={subtitulo}>Nueva venta</h2>

                    <form onSubmit={manejarCrearVenta} style={formularioEstilo}>
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

                        <label style={label}>Líneas de venta</label>

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
                                    {productosActivos.map((producto) => (
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
                                    placeholder="Cantidad"
                                    required
                                />

                                <input
                                    style={input}
                                    type="number"
                                    min="0"
                                    step="0.01"
                                    value={linea.precioUnitario}
                                    onChange={(e) =>
                                        actualizarLinea(index, "precioUnitario", e.target.value)
                                    }
                                    placeholder="Precio unitario"
                                    required
                                />

                                <div style={subtotalBox}>
                                    Subtotal línea: <strong>₡{subtotalLinea(linea).toFixed(2)}</strong>
                                </div>

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
                            <p style={resumenLinea}>
                                <span>Subtotal:</span>
                                <strong>₡{resumen.subtotal.toFixed(2)}</strong>
                            </p>
                            <p style={resumenLinea}>
                                <span>IVA:</span>
                                <strong>₡{resumen.IVA.toFixed(2)}</strong>
                            </p>
                            <p style={resumenTotal}>
                                <span>Total:</span>
                                <strong>₡{resumen.total.toFixed(2)}</strong>
                            </p>
                        </div>

                        <button style={botonPrimario} type="submit" disabled={guardando}>
                            {guardando ? "Registrando..." : "Registrar venta"}
                        </button>
                    </form>
                </div>

                <div style={columnaDerecha}>
                    <div style={card}>
                        <h2 style={subtitulo}>Buscar factura</h2>

                        <div style={busquedaBox}>
                            <input
                                style={input}
                                type="number"
                                placeholder="Número de factura"
                                value={numeroBusqueda}
                                onChange={(e) => setNumeroBusqueda(e.target.value)}
                            />
                            <button style={botonSecundario} onClick={manejarBuscarVenta}>
                                Abrir
                            </button>
                        </div>
                    </div>

                    <div style={card}>
                        <div style={cabeceraListado}>
                            <h2 style={subtitulo}>Ventas recientes</h2>
                            <span style={contadorBadge}>{ventas.length} registradas</span>
                        </div>

                        {cargando ? (
                            <p>Cargando ventas...</p>
                        ) : ventasOrdenadas.length === 0 ? (
                            <p>No hay ventas registradas.</p>
                        ) : (
                            <div style={listaVentas}>
                                {ventasOrdenadas.map((venta) => (
                                    <div key={venta.numero} style={ventaItem}>
                                        <div>
                                            <p style={ventaTitulo}>Factura #{venta.numero}</p>
                                            <p style={ventaTexto}>
                                                Cliente: {nombreClientePorId(venta.clienteId)}
                                            </p>
                                            <p style={ventaTexto}>
                                                Fecha: {formatearFecha(venta.fecha)}
                                            </p>
                                        </div>

                                        <div style={ventaLadoDerecho}>
                                            <p style={ventaMonto}>₡{Number(venta.total).toFixed(2)}</p>
                                            <button
                                                style={botonVer}
                                                onClick={() => abrirFactura(venta)}
                                            >
                                                Ver / Imprimir
                                            </button>
                                        </div>
                                    </div>
                                ))}
                            </div>
                        )}
                    </div>
                </div>
            </div>

            <FacturaPrintModal
                abierto={modalFacturaAbierto}
                onClose={() => setModalFacturaAbierto(false)}
                onPrint={imprimirFactura}
                venta={ventaSeleccionada}
                clienteNombre={
                    ventaSeleccionada ? nombreClientePorId(ventaSeleccionada.clienteId) : ""
                }
                detallesConNombre={detallesConNombre}
            />
        </div>
    );
}

function formatearFecha(fecha) {
    if (!fecha) return "";
    const date = new Date(fecha);
    if (isNaN(date.getTime())) return fecha;
    return date.toLocaleString();
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

const textoSecundario = {
    margin: "6px 0 0 0",
    color: "#6b7280",
    fontSize: "14px",
};

const gridPrincipal = {
    display: "grid",
    gridTemplateColumns: "1.2fr 0.9fr",
    gap: "24px",
    alignItems: "start",
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
    gridTemplateColumns: "1.3fr 120px 150px 1fr 100px",
    gap: "10px",
    backgroundColor: "#f9fafb",
    alignItems: "center",
};

const subtotalBox = {
    fontSize: "14px",
    color: "#111827",
};

const resumenCard = {
    border: "1px solid #d1d5db",
    borderRadius: "10px",
    padding: "14px",
    backgroundColor: "#f9fafb",
};

const resumenLinea = {
    display: "flex",
    justifyContent: "space-between",
    margin: "6px 0",
};

const resumenTotal = {
    display: "flex",
    justifyContent: "space-between",
    margin: "10px 0 0 0",
    paddingTop: "10px",
    borderTop: "2px solid #111827",
    fontSize: "18px",
};

const busquedaBox = {
    display: "flex",
    gap: "12px",
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

const listaVentas = {
    display: "flex",
    flexDirection: "column",
    gap: "12px",
};

const ventaItem = {
    border: "1px solid #e5e7eb",
    borderRadius: "10px",
    padding: "14px",
    display: "flex",
    justifyContent: "space-between",
    gap: "12px",
    alignItems: "center",
    backgroundColor: "#f9fafb",
};

const ventaTitulo = {
    margin: "0 0 6px 0",
    fontWeight: "bold",
};

const ventaTexto = {
    margin: "2px 0",
    color: "#4b5563",
    fontSize: "14px",
};

const ventaLadoDerecho = {
    textAlign: "right",
    minWidth: "160px",
};

const ventaMonto = {
    margin: "0 0 8px 0",
    fontWeight: "bold",
    fontSize: "18px",
};

const botonVer = {
    backgroundColor: "#111827",
    color: "white",
    border: "none",
    padding: "8px 12px",
    borderRadius: "8px",
    cursor: "pointer",
};

export default VentasPage;