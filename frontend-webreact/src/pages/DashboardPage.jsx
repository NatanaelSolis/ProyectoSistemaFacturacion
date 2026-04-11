import { useEffect, useMemo, useState } from "react";
import { obtenerProductos } from "../services/productosApi";
import { obtenerClientes } from "../services/clientesApi";
import { obtenerVentas } from "../services/ventasApi";

function DashboardPage() {
    const [productos, setProductos] = useState([]);
    const [clientes, setClientes] = useState([]);
    const [ventas, setVentas] = useState([]);
    const [cargando, setCargando] = useState(true);
    const [error, setError] = useState("");

    useEffect(() => {
        cargarDashboard();
    }, []);

    async function cargarDashboard() {
        try {
            setCargando(true);
            setError("");

            const [productosData, clientesData, ventasData] = await Promise.all([
                obtenerProductos(),
                obtenerClientes(),
                obtenerVentas(),
            ]);

            setProductos(productosData);
            setClientes(clientesData);
            setVentas(ventasData);
        } catch (err) {
            setError(err.message);
        } finally {
            setCargando(false);
        }
    }

    const resumen = useMemo(() => {
        const totalProductos = productos.length;
        const productosActivos = productos.filter((p) => p.estado === "Activo").length;
        const productosInactivos = productos.filter((p) => p.estado === "Inactivo").length;
        const totalClientes = clientes.length;
        const totalVentas = ventas.length;
        const montoTotalVendido = ventas.reduce(
            (acumulado, venta) => acumulado + Number(venta.total || 0),
            0
        );

        const ultimasVentas = [...ventas]
            .sort((a, b) => Number(b.numero) - Number(a.numero))
            .slice(0, 5);

        return {
            totalProductos,
            productosActivos,
            productosInactivos,
            totalClientes,
            totalVentas,
            montoTotalVendido,
            ultimasVentas,
        };
    }, [productos, clientes, ventas]);

    if (cargando) {
        return <div style={contenedor}>Cargando dashboard...</div>;
    }

    return (
        <div style={contenedor}>
            <div style={encabezado}>
                <div>
                    <h1 style={{ margin: 0 }}>Dashboard</h1>
                    <p style={textoSecundario}>
                        Resumen general del sistema de facturación
                    </p>
                </div>

                <button style={botonSecundario} onClick={cargarDashboard}>
                    Recargar
                </button>
            </div>

            {error && <p style={textoError}>Error: {error}</p>}

            <div style={gridMetricas}>
                <div style={cardMetrica}>
                    <p style={tituloMetrica}>Productos</p>
                    <h2 style={valorMetrica}>{resumen.totalProductos}</h2>
                </div>

                <div style={cardMetrica}>
                    <p style={tituloMetrica}>Productos activos</p>
                    <h2 style={valorMetrica}>{resumen.productosActivos}</h2>
                </div>

                <div style={cardMetrica}>
                    <p style={tituloMetrica}>Productos inactivos</p>
                    <h2 style={valorMetrica}>{resumen.productosInactivos}</h2>
                </div>

                <div style={cardMetrica}>
                    <p style={tituloMetrica}>Clientes</p>
                    <h2 style={valorMetrica}>{resumen.totalClientes}</h2>
                </div>

                <div style={cardMetrica}>
                    <p style={tituloMetrica}>Ventas</p>
                    <h2 style={valorMetrica}>{resumen.totalVentas}</h2>
                </div>

                <div style={cardMetricaDestacada}>
                    <p style={tituloMetricaClaro}>Monto total vendido</p>
                    <h2 style={valorMetricaClaro}>
                        ₡{resumen.montoTotalVendido.toFixed(2)}
                    </h2>
                </div>
            </div>

            <div style={cardGrande}>
                <div style={cabeceraCard}>
                    <h2 style={subtitulo}>Últimas ventas</h2>
                    <span style={badge}>{resumen.ultimasVentas.length} visibles</span>
                </div>

                {resumen.ultimasVentas.length === 0 ? (
                    <p>No hay ventas registradas todavía.</p>
                ) : (
                    <div style={tablaContenedor}>
                        <table style={tabla}>
                            <thead>
                                <tr>
                                    <th style={th}>Número</th>
                                    <th style={th}>Cliente ID</th>
                                    <th style={th}>Fecha</th>
                                    <th style={th}>Subtotal</th>
                                    <th style={th}>IVA</th>
                                    <th style={th}>Total</th>
                                    <th style={th}>Estado</th>
                                </tr>
                            </thead>
                            <tbody>
                                {resumen.ultimasVentas.map((venta) => (
                                    <tr key={venta.numero}>
                                        <td style={td}>{venta.numero}</td>
                                        <td style={td}>{venta.clienteId}</td>
                                        <td style={td}>{formatearFecha(venta.fecha)}</td>
                                        <td style={td}>₡{Number(venta.subtotal || 0).toFixed(2)}</td>
                                        <td style={td}>
                                            ₡{Number((venta.IVA ?? venta.iva) || 0).toFixed(2)}
                                        </td>
                                        <td style={td}>₡{Number(venta.total || 0).toFixed(2)}</td>
                                        <td style={td}>{venta.estado}</td>
                                    </tr>
                                ))}
                            </tbody>
                        </table>
                    </div>
                )}
            </div>
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

const gridMetricas = {
    display: "grid",
    gridTemplateColumns: "repeat(3, minmax(0, 1fr))",
    gap: "18px",
    marginBottom: "24px",
};

const cardMetrica = {
    backgroundColor: "white",
    borderRadius: "14px",
    padding: "20px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.08)",
    border: "1px solid #e5e7eb",
};

const cardMetricaDestacada = {
    backgroundColor: "#111827",
    color: "white",
    borderRadius: "14px",
    padding: "20px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.08)",
    border: "1px solid #111827",
};

const tituloMetrica = {
    margin: 0,
    color: "#6b7280",
    fontSize: "14px",
};

const valorMetrica = {
    margin: "12px 0 0 0",
    fontSize: "32px",
};

const tituloMetricaClaro = {
    margin: 0,
    color: "#d1d5db",
    fontSize: "14px",
};

const valorMetricaClaro = {
    margin: "12px 0 0 0",
    fontSize: "32px",
};

const cardGrande = {
    backgroundColor: "white",
    borderRadius: "14px",
    padding: "20px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.08)",
    border: "1px solid #e5e7eb",
};

const cabeceraCard = {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    marginBottom: "16px",
};

const subtitulo = {
    margin: 0,
};

const badge = {
    backgroundColor: "#dbeafe",
    color: "#1d4ed8",
    padding: "6px 10px",
    borderRadius: "999px",
    fontSize: "12px",
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

const tablaContenedor = {
    overflowX: "auto",
};

const tabla = {
    width: "100%",
    borderCollapse: "collapse",
};

const th = {
    border: "1px solid #d1d5db",
    padding: "10px",
    backgroundColor: "#111827",
    color: "white",
    textAlign: "left",
    fontSize: "14px",
};

const td = {
    border: "1px solid #d1d5db",
    padding: "10px",
    fontSize: "14px",
};

const textoError = {
    color: "#dc2626",
    fontWeight: "bold",
};

export default DashboardPage;