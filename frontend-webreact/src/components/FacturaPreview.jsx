import { empresaInfo } from "../config/empresa";

function FacturaPreview({ venta, clienteNombre, detallesConNombre }) {
    if (!venta) {
        return (
            <div style={cardFactura}>
                <h2 style={tituloFactura}>Vista previa de factura</h2>
                <p style={textoSuave}>Selecciona o registra una venta para ver la factura.</p>
            </div>
        );
    }

    return (
        <div style={cardFactura}>
            <div style={encabezadoFactura}>
                <div>
                    <h2 style={tituloFactura}>{empresaInfo.nombre}</h2>
                    <p style={textoEmpresa}>{empresaInfo.subtitulo}</p>
                    <p style={textoEmpresa}>Tel: {empresaInfo.telefono}</p>
                    <p style={textoEmpresa}>Correo: {empresaInfo.correo}</p>
                    <p style={textoEmpresa}>Dirección: {empresaInfo.direccion}</p>
                    <p style={textoEmpresa}>Cédula: {empresaInfo.cedula}</p>
                </div>

                <div style={bloqueFactura}>
                    <p style={datoFactura}><strong>Factura #:</strong> {venta.numero}</p>
                    <p style={datoFactura}><strong>Fecha:</strong> {formatearFecha(venta.fecha)}</p>
                    <p style={datoFactura}><strong>Estado:</strong> {venta.estado}</p>
                </div>
            </div>

            <hr style={linea} />

            <div style={bloqueCliente}>
                <p style={datoFactura}><strong>Cliente:</strong> {clienteNombre || `Cliente ${venta.clienteId}`}</p>
                <p style={datoFactura}><strong>ID Cliente:</strong> {venta.clienteId}</p>
            </div>

            <div style={tablaContenedor}>
                <table style={tabla}>
                    <thead>
                        <tr>
                            <th style={th}>Producto</th>
                            <th style={th}>Cant.</th>
                            <th style={th}>P. Unitario</th>
                            <th style={th}>Subtotal</th>
                        </tr>
                    </thead>
                    <tbody>
                        {(detallesConNombre || []).map((d, i) => (
                            <tr key={i}>
                                <td style={td}>{d.nombreProducto || `Código ${d.productoCodigo}`}</td>
                                <td style={td}>{d.cantidad}</td>
                                <td style={td}>₡{Number(d.precioUnitario).toFixed(2)}</td>
                                <td style={td}>₡{Number(d.subtotal).toFixed(2)}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>

            <div style={resumen}>
                <p style={totalLinea}><span>Subtotal:</span><strong>₡{Number(venta.subtotal || 0).toFixed(2)}</strong></p>
                <p style={totalLinea}><span>IVA:</span><strong>₡{Number((venta.IVA ?? venta.iva) || 0).toFixed(2)}</strong></p>
                <p style={totalFinal}><span>Total:</span><strong>₡{Number(venta.total || 0).toFixed(2)}</strong></p>
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

const cardFactura = {
    backgroundColor: "white",
    borderRadius: "14px",
    padding: "20px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.08)",
    border: "1px solid #e5e7eb",
};

const encabezadoFactura = {
    display: "flex",
    justifyContent: "space-between",
    gap: "16px",
    flexWrap: "wrap",
};

const tituloFactura = {
    margin: 0,
    fontSize: "24px",
};

const textoEmpresa = {
    margin: "4px 0",
    color: "#4b5563",
    fontSize: "14px",
};

const bloqueFactura = {
    minWidth: "220px",
    backgroundColor: "#f9fafb",
    borderRadius: "10px",
    padding: "12px",
};

const bloqueCliente = {
    marginBottom: "16px",
};

const datoFactura = {
    margin: "6px 0",
};

const linea = {
    border: "none",
    borderTop: "1px solid #e5e7eb",
    margin: "18px 0",
};

const tablaContenedor = {
    overflowX: "auto",
};

const tabla = {
    width: "100%",
    borderCollapse: "collapse",
};

const th = {
    textAlign: "left",
    padding: "10px",
    backgroundColor: "#111827",
    color: "white",
    fontSize: "14px",
};

const td = {
    borderBottom: "1px solid #e5e7eb",
    padding: "10px",
    fontSize: "14px",
};

const resumen = {
    marginTop: "18px",
    marginLeft: "auto",
    maxWidth: "320px",
};

const totalLinea = {
    display: "flex",
    justifyContent: "space-between",
    margin: "8px 0",
};

const totalFinal = {
    display: "flex",
    justifyContent: "space-between",
    margin: "12px 0 0 0",
    paddingTop: "12px",
    borderTop: "2px solid #111827",
    fontSize: "18px",
};

const textoSuave = {
    color: "#6b7280",
};

export default FacturaPreview;