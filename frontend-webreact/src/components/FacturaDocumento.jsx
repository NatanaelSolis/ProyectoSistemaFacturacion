import { empresaInfo } from "../config/empresa";

function FacturaDocumento({ venta, clienteNombre, detallesConNombre }) {
    if (!venta) return null;

    return (
        <div style={contenedor}>
            <div style={encabezado}>
                <div>
                    <h1 style={tituloEmpresa}>{empresaInfo.nombre}</h1>
                    <p style={textoEmpresa}>{empresaInfo.subtitulo}</p>
                    <p style={textoEmpresa}>Tel: {empresaInfo.telefono}</p>
                    <p style={textoEmpresa}>Correo: {empresaInfo.correo}</p>
                    <p style={textoEmpresa}>Dirección: {empresaInfo.direccion}</p>
                    <p style={textoEmpresa}>Cédula: {empresaInfo.cedula}</p>
                </div>

                <div style={bloqueFactura}>
                    <h2 style={tituloFactura}>FACTURA</h2>
                    <p style={dato}><strong>Número:</strong> {venta.numero}</p>
                    <p style={dato}><strong>Fecha:</strong> {formatearFecha(venta.fecha)}</p>
                    <p style={dato}><strong>Estado:</strong> {venta.estado}</p>
                </div>
            </div>

            <div style={separador} />

            <div style={bloqueCliente}>
                <p style={dato}><strong>Cliente:</strong> {clienteNombre || `Cliente ${venta.clienteId}`}</p>
                <p style={dato}><strong>ID Cliente:</strong> {venta.clienteId}</p>
            </div>

            <table style={tabla}>
                <thead>
                    <tr>
                        <th style={th}>Producto</th>
                        <th style={th}>Cantidad</th>
                        <th style={th}>Precio unitario</th>
                        <th style={th}>Subtotal</th>
                    </tr>
                </thead>
                <tbody>
                    {(detallesConNombre || []).map((detalle, index) => (
                        <tr key={index}>
                            <td style={td}>{detalle.nombreProducto || `Código ${detalle.productoCodigo}`}</td>
                            <td style={td}>{detalle.cantidad}</td>
                            <td style={td}>₡{Number(detalle.precioUnitario || 0).toFixed(2)}</td>
                            <td style={td}>₡{Number(detalle.subtotal || 0).toFixed(2)}</td>
                        </tr>
                    ))}
                </tbody>
            </table>

            <div style={totales}>
                <p style={lineaTotal}>
                    <span>Subtotal:</span>
                    <strong>₡{Number(venta.subtotal || 0).toFixed(2)}</strong>
                </p>
                <p style={lineaTotal}>
                    <span>IVA:</span>
                    <strong>₡{Number((venta.IVA ?? venta.iva) || 0).toFixed(2)}</strong>
                </p>
                <p style={lineaTotalFinal}>
                    <span>Total:</span>
                    <strong>₡{Number(venta.total || 0).toFixed(2)}</strong>
                </p>
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
    width: "100%",
    maxWidth: "900px",
    margin: "0 auto",
    backgroundColor: "white",
    color: "#111827",
    padding: "32px",
    boxSizing: "border-box",
    fontFamily: "Arial, sans-serif",
};

const encabezado = {
    display: "flex",
    justifyContent: "space-between",
    gap: "24px",
    alignItems: "flex-start",
};

const tituloEmpresa = {
    margin: 0,
    fontSize: "30px",
};

const tituloFactura = {
    margin: "0 0 12px 0",
    fontSize: "26px",
    textAlign: "right",
};

const textoEmpresa = {
    margin: "4px 0",
    color: "#374151",
    fontSize: "14px",
};

const bloqueFactura = {
    minWidth: "260px",
    textAlign: "right",
};

const bloqueCliente = {
    marginBottom: "20px",
};

const dato = {
    margin: "6px 0",
    fontSize: "14px",
};

const separador = {
    height: "1px",
    backgroundColor: "#d1d5db",
    margin: "24px 0",
};

const tabla = {
    width: "100%",
    borderCollapse: "collapse",
    marginTop: "12px",
};

const th = {
    backgroundColor: "#111827",
    color: "white",
    textAlign: "left",
    padding: "12px",
    fontSize: "14px",
};

const td = {
    borderBottom: "1px solid #e5e7eb",
    padding: "12px",
    fontSize: "14px",
};

const totales = {
    maxWidth: "320px",
    marginLeft: "auto",
    marginTop: "24px",
};

const lineaTotal = {
    display: "flex",
    justifyContent: "space-between",
    margin: "8px 0",
    fontSize: "15px",
};

const lineaTotalFinal = {
    display: "flex",
    justifyContent: "space-between",
    margin: "14px 0 0 0",
    paddingTop: "12px",
    borderTop: "2px solid #111827",
    fontSize: "18px",
};

export default FacturaDocumento;