import { useEffect, useState } from "react";
import {
    obtenerClientes,
    crearCliente,
    eliminarCliente,
} from "../services/clientesApi";

const clienteInicial = {
    id: "",
    nombre: "",
    correo: "",
    telefono: "",
    direccion: "",
};

function ClientesPage() {
    const [clientes, setClientes] = useState([]);
    const [cargando, setCargando] = useState(true);
    const [error, setError] = useState("");
    const [mensaje, setMensaje] = useState("");
    const [formulario, setFormulario] = useState(clienteInicial);
    const [guardando, setGuardando] = useState(false);

    useEffect(() => {
        cargarClientes();
    }, []);

    async function cargarClientes() {
        try {
            setCargando(true);
            setError("");
            const data = await obtenerClientes();
            setClientes(data);
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

            const nuevoCliente = {
                id: Number(formulario.id),
                nombre: formulario.nombre,
                correo: formulario.correo,
                telefono: formulario.telefono,
                direccion: formulario.direccion,
            };

            await crearCliente(nuevoCliente);
            setMensaje("Cliente creado correctamente.");
            setFormulario(clienteInicial);
            await cargarClientes();
        } catch (err) {
            setError(err.message);
        } finally {
            setGuardando(false);
        }
    }

    async function manejarEliminar(id) {
        const confirmado = window.confirm(`¿Deseas eliminar el cliente ${id}?`);

        if (!confirmado) return;

        try {
            setError("");
            setMensaje("");
            await eliminarCliente(id);
            setMensaje("Cliente eliminado correctamente.");
            await cargarClientes();
        } catch (err) {
            setError(err.message);
        }
    }

    return (
        <div style={contenedor}>
            <div style={encabezado}>
                <div>
                    <h1 style={{ margin: 0 }}>Clientes</h1>
                    <p style={textoSecundario}>Administra tus clientes</p>
                </div>

                <button style={botonSecundario} onClick={cargarClientes}>
                    Recargar
                </button>
            </div>

            <div style={grid}>
                <div style={card}>
                    <h2 style={subtitulo}>Registrar cliente</h2>

                    <form onSubmit={manejarSubmit} style={formularioEstilo}>
                        <input
                            style={input}
                            type="number"
                            name="id"
                            placeholder="ID"
                            value={formulario.id}
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
                            type="email"
                            name="correo"
                            placeholder="Correo"
                            value={formulario.correo}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="text"
                            name="telefono"
                            placeholder="Teléfono"
                            value={formulario.telefono}
                            onChange={manejarCambio}
                            required
                        />

                        <input
                            style={input}
                            type="text"
                            name="direccion"
                            placeholder="Dirección"
                            value={formulario.direccion}
                            onChange={manejarCambio}
                            required
                        />

                        <button style={botonPrimario} type="submit" disabled={guardando}>
                            {guardando ? "Guardando..." : "Crear cliente"}
                        </button>
                    </form>
                </div>

                <div style={card}>
                    <h2 style={subtitulo}>Listado de clientes</h2>

                    {error && <p style={textoError}>Error: {error}</p>}
                    {mensaje && <p style={textoExito}>{mensaje}</p>}

                    {cargando ? (
                        <p>Cargando clientes...</p>
                    ) : clientes.length === 0 ? (
                        <p>No hay clientes registrados.</p>
                    ) : (
                        <div style={tablaContenedor}>
                            <table style={tabla}>
                                <thead>
                                    <tr>
                                        <th style={th}>ID</th>
                                        <th style={th}>Nombre</th>
                                        <th style={th}>Correo</th>
                                        <th style={th}>Teléfono</th>
                                        <th style={th}>Dirección</th>
                                        <th style={th}>Acciones</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    {clientes.map((cliente) => (
                                        <tr key={cliente.id}>
                                            <td style={td}>{cliente.id}</td>
                                            <td style={td}>{cliente.nombre}</td>
                                            <td style={td}>{cliente.correo}</td>
                                            <td style={td}>{cliente.telefono}</td>
                                            <td style={td}>{cliente.direccion}</td>
                                            <td style={td}>
                                                <button
                                                    style={botonEliminar}
                                                    onClick={() => manejarEliminar(cliente.id)}
                                                >
                                                    Eliminar
                                                </button>
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

const botonEliminar = {
    backgroundColor: "#dc2626",
    color: "white",
    border: "none",
    padding: "8px 10px",
    borderRadius: "6px",
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

const textoExito = {
    color: "#16a34a",
    fontWeight: "bold",
};

export default ClientesPage;