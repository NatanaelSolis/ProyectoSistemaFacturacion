import { useState } from "react";
import ProductosPage from "./pages/ProductosPage";
import ClientesPage from "./pages/ClientesPage";
import VentasPage from "./pages/VentasPage";

function App() {
    const [modulo, setModulo] = useState("productos");

    return (
        <div>
            <nav style={nav}>
                <button
                    style={modulo === "productos" ? botonActivo : botonNav}
                    onClick={() => setModulo("productos")}
                >
                    Productos
                </button>

                <button
                    style={modulo === "clientes" ? botonActivo : botonNav}
                    onClick={() => setModulo("clientes")}
                >
                    Clientes
                </button>

                <button
                    style={modulo === "ventas" ? botonActivo : botonNav}
                    onClick={() => setModulo("ventas")}
                >
                    Ventas
                </button>
            </nav>

            {modulo === "productos" && <ProductosPage />}
            {modulo === "clientes" && <ClientesPage />}
            {modulo === "ventas" && <VentasPage />}
        </div>
    );
}

const nav = {
    display: "flex",
    gap: "12px",
    padding: "16px 24px",
    backgroundColor: "#111827",
};

const botonNav = {
    backgroundColor: "#374151",
    color: "white",
    border: "none",
    padding: "10px 14px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

const botonActivo = {
    backgroundColor: "#2563eb",
    color: "white",
    border: "none",
    padding: "10px 14px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

export default App;