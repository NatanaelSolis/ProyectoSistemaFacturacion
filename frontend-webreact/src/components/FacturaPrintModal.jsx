import FacturaDocumento from "./FacturaDocumento";

function FacturaPrintModal({
    abierto,
    onClose,
    onPrint,
    venta,
    clienteNombre,
    detallesConNombre,
}) {
    if (!abierto || !venta) return null;

    return (
        <div style={overlay}>
            <div style={modal}>
                <div style={barra}>
                    <h2 style={{ margin: 0 }}>Vista de factura</h2>

                    <div style={acciones}>
                        <button style={botonSecundario} onClick={onPrint}>
                            Imprimir
                        </button>
                        <button style={botonCerrar} onClick={onClose}>
                            Cerrar
                        </button>
                    </div>
                </div>

                <div id="factura-print-area" style={contenido}>
                    <FacturaDocumento
                        venta={venta}
                        clienteNombre={clienteNombre}
                        detallesConNombre={detallesConNombre}
                    />
                </div>
            </div>
        </div>
    );
}

const overlay = {
    position: "fixed",
    inset: 0,
    backgroundColor: "rgba(0,0,0,0.55)",
    display: "flex",
    justifyContent: "center",
    alignItems: "center",
    zIndex: 9999,
    padding: "24px",
};

const modal = {
    width: "100%",
    maxWidth: "1100px",
    maxHeight: "90vh",
    backgroundColor: "#f9fafb",
    borderRadius: "16px",
    overflow: "hidden",
    boxShadow: "0 20px 60px rgba(0,0,0,0.25)",
    display: "flex",
    flexDirection: "column",
};

const barra = {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    padding: "16px 20px",
    backgroundColor: "#111827",
    color: "white",
};

const acciones = {
    display: "flex",
    gap: "10px",
};

const contenido = {
    overflowY: "auto",
    padding: "20px",
};

const botonSecundario = {
    backgroundColor: "#2563eb",
    color: "white",
    border: "none",
    padding: "10px 14px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

const botonCerrar = {
    backgroundColor: "#dc2626",
    color: "white",
    border: "none",
    padding: "10px 14px",
    borderRadius: "8px",
    cursor: "pointer",
    fontWeight: "bold",
};

export default FacturaPrintModal;