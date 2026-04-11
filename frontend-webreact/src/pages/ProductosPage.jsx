import { useEffect, useMemo, useState } from "react";
import {
    obtenerProductos,
    crearProducto,
    desactivarProducto,
    activarProducto,
} from "../services/productosApi";

const productoInicial = {
    codigo: "",
    nombre: "",
    categoria: "",
    marca: "",
    precio: "",
    stock: "",
    estado: "Activo",
};

function ProductosPage() {
    const [productos, setProductos] = useState([]);
    const [cargando, setCargando] = useState(true);
    const [error, setError] = useState("");
    const [mensaje, setMensaje] = useState("");
    const [formulario, setFormulario] = useState(productoInicial);
    const [guardando, setGuardando] = useState(false);
    const [filtroEstado, setFiltroEstado] = useState("Todos");

    useEffect(() => {
        cargarProductos();
    }, []);

    async function cargarProductos() {
        try {
            setCargando(true);
            setError("");
            const data = await obtenerProductos();
            setProductos(data);
        } catch (err) {
            setError(err.message);
        } finally {
            setCargando(false);
        }
    }

    function manejarCambio(e) {
        const { name, value } = e.target;
        setFormulario((prev) => ({
            ...prev,
            [name]: value,
        }));
    }

    async function manejarSubmit(e) {
        e.preventDefault();

        try {
            setGuardando(true);
            setError("");
            setMensaje("");

            const nuevoProducto = {
                codigo: Number(formulario.codigo),
                nombre: formulario.nombre,
                categoria: formulario.categoria,
                marca: formulario.marca,
                precio: Number(formulario.precio),
                stock: Number(formulario.stock),
                estado: formulario.estado,
            };

            await crearProducto(nuevoProducto);
            setMensaje("Producto creado correctamente.");
            setFormulario(productoInicial);
            await cargarProductos();
            setFiltroEstado("Todos");
        } catch (err) {
            setError(err.message);
        } finally {
            setGuardando(false);
        }
    }

    async function manejarDesactivar(codigo) {
        const confirmado = window.confirm(
            `¿Deseas desactivar el producto ${codigo}?`
        );

        if (!confirmado) return;

        try {
            setError("");
            setMensaje("");
            await desactivarProducto(codigo);
            setMensaje("Producto desactivado correctamente.");
            await cargarProductos();
        } catch (err) {
            setError(err.message);
        }
    }

    async function manejarActivar(codigo) {
        try {
            setError("");
            setMensaje("");
            await activarProducto(codigo);
            setMensaje("Producto activado correctamente.");
            await cargarProductos();
        } catch (err) {
            setError(err.message);
        }
    }

    const productosFiltrados = useMemo(() => {
        if (filtroEstado === "Todos") return productos;
        return productos.filter((p) => p.estado === filtroEstado);
    }, [productos, filtroEstado]);

    const totalActivos = productos.filter((p) => p.estado === "Activo").length;
    const totalInactivos = productos.filter((p) => p.estado === "Inactivo").length;

    return (
        <div style={contenedor}>
            <div style={encabezado}>
                <div>
                    <h1 style={{ margin: 0 }}>Productos</h1>
                    <p style={textoSecundario}>
                        Administra tu catálogo de perfumes
                    </p>
                </div>

                <button style={botonSecundario} onClick={cargarProductos}>
                    Recargar
                </button>
            </div>

            <div style={grid}>
                <div style={card}>
                    <h2 style={subtitulo}>Registrar producto</h2>

                    <form onSubmit={manejarSubmit} style={formularioEstilo}>
                        <input
                            style={input}
                            type="number"
                            name="codigo"
                            placeholder="Código"
                            value={formulario.codigo}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="text"
                            name="nombre"
                            placeholder="Nombre"
                            value={formulario.nombre}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="text"
                            name="categoria"
                            placeholder="Categoría"
                            value={formulario.categoria}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="text"
                            name="marca"
                            placeholder="Marca"
                            value={formulario.marca}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="number"
                            name="precio"
                            placeholder="Precio"
                            value={formulario.precio}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="number"
                            name="stock"
                            placeholder="Stock"
                            value={formulario.stock}
                            onChange={manejarCambio}
                            required
                        />

                        <select
                            style={input}
                            name="estado"
                            value={formulario.estado}
                            onChange={manejarCambio}
                        >
                            <option value="Activo">Activo</option>
                            <option value="Inactivo">Inactivo</option>
                        </select>

                        <button style={botonPrimario} type="submit" disabled={guardando}>
                            {guardando ? "Guardando..." : "Crear producto"}
                        </button>
                    </form>
                </div>

                <div style={card}>
                    <div style={barraSuperiorListado}>
                        <div>
                            <h2 style={subtituloListado}>Listado de productos</h2>
                            <p style={textoSecundario}>
                                Activos: {totalActivos} | Inactivos: {totalInactivos}
                            </p>
                        </div>

                        <div style={grupoFiltros}>
                            <button
                                style={filtroEstado === "Todos" ? botonFiltroActivo : botonFiltro}
                                onClick={() => setFiltroEstado("Todos")}
                            >
                                Todos
                            </button>

                            <button
                                style={filtroEstado === "Activo" ? botonFiltroActivo : botonFiltro}
                                onClick={() => setFiltroEstado("Activo")}
                            >
                                Activos
                            </button>

                            <button
                                style={filtroEstado === "Inactivo" ? botonFiltroActivo : botonFiltro}
                                onClick={() => setFiltroEstado("Inactivo")}
                            >
                                Inactivos
                            </button>
                        </div>
                    </div>

                    {error && <p style={textoError}>Error: {error}</p>}
                    {mensaje && <p style={textoExito}>{mensaje}</p>}

                    {cargando ? (
                        <p>Cargando productos...</p>
                    ) : productosFiltrados.length === 0 ? (
                        <p>No hay productos para este filtro.</p>
                    ) : (
                        <div style={tablaContenedor}>
                            <table style={tabla}>
                                <thead>
                                    <tr>
                                        <th style={th}>Código</th>
                                        <th style={th}>Nombre</th>
                                        <th style={th}>Categoría</th>
                                        <th style={th}>Marca</th>
                                        <th style={th}>Precio</th>
                                        <th style={th}>Stock</th>
                                        <th style={th}>Estado</th>
                                        <th style={th}>Acciones</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    {productosFiltrados.map((producto) => (
                                        <tr key={producto.codigo}>
                                            <td style={td}>{producto.codigo}</td>
                                            <td style={td}>{producto.nombre}</td>
                                            <td style={td}>{producto.categoria}</td>
                                            <td style={td}>{producto.marca}</td>
                                            <td style={td}>{producto.precio}</td>
                                            <td style={td}>{producto.stock}</td>
                                            <td style={td}>
                                                <span
                                                    style={
                                                        producto.estado === "Activo"
                                                            ? badgeActivo
                                                            : badgeInactivo
                                                    }
                                                >
                                                    {producto.estado}
                                                </span>
                                            </td>
                                            <td style={td}>
                                                {producto.estado === "Activo" ? (
                                                    <button
                                                        style={botonDesactivar}
                                                        onClick={() => manejarDesactivar(producto.codigo)}
                                                    >
                                                        Desactivar
                                                    </button>
                                                ) : (
                                                    <button
                                                        style={botonActivar}
                                                        onClick={() => manejarActivar(producto.codigo)}
                                                    >
                                                        Activar
                                                    </button>
                                                )}
                                            </td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    )}
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

const grid = {
    display: "grid",
    gridTemplateColumns: "360px 1fr",
    gap: "24px",
};

const card = {
    backgroundColor: "white",
    borderRadius: "12px",
    padding: "20px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.08)",
};

const subtitulo = {
    marginTop: 0,
    marginBottom: "16px",
};

const subtituloListado = {
    marginTop: 0,
    marginBottom: "6px",
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

const input = {
    padding: "10px 12px",
    border: "1px solid #d1d5db",
    borderRadius: "8px",
    fontSize: "14px",
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

const barraSuperiorListado = {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    gap: "16px",
    marginBottom: "16px",
    flexWrap: "wrap",
};

const grupoFiltros = {
    display: "flex",
    gap: "8px",
    flexWrap: "wrap",
};

const botonFiltro = {
    backgroundColor: "#e5e7eb",
    color: "#111827",
    border: "none",
    padding: "8px 12px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

const botonFiltroActivo = {
    backgroundColor: "#111827",
    color: "white",
    border: "none",
    padding: "8px 12px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

const botonDesactivar = {
    backgroundColor: "#dc2626",
    color: "white",
    border: "none",
    padding: "8px 10px",
    borderRadius: "6px",
    cursor: "pointer",
};

const botonActivar = {
    backgroundColor: "#16a34a",
    color: "white",
    border: "none",
    padding: "8px 10px",
    borderRadius: "6px",
    cursor: "pointer",
};

const badgeActivo = {
    backgroundColor: "#dcfce7",
    color: "#166534",
    padding: "4px 8px",
    borderRadius: "999px",
    fontSize: "12px",
    fontWeight: "bold",
};

const badgeInactivo = {
    backgroundColor: "#fee2e2",
    color: "#991b1b",
    padding: "4px 8px",
    borderRadius: "999px",
    fontSize: "12px",
    fontWeight: "bold",
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

const textoExito = {
    color: "#16a34a",
    fontWeight: "bold",
};

export default ProductosPage;