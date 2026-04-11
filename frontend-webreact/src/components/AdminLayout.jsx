function AdminLayout({ modulo, setModulo, children }) {
    const opciones = [
        { key: "dashboard", label: "Dashboard" },
        { key: "productos", label: "Productos" },
        { key: "clientes", label: "Clientes" },
        { key: "ventas", label: "Ventas" },
    ];

    return (
        <div style={layout}>
            <aside style={sidebar}>
                <div style={brand}>
                    <h2 style={brandTitle}>FRARENA</h2>
                    <p style={brandSubtitle}>Panel administrativo</p>
                </div>

                <nav style={nav}>
                    {opciones.map((opcion) => (
                        <button
                            key={opcion.key}
                            style={modulo === opcion.key ? botonActivo : botonNav}
                            onClick={() => setModulo(opcion.key)}
                        >
                            {opcion.label}
                        </button>
                    ))}
                </nav>
            </aside>

            <main style={mainContent}>
                <header style={topbar}>
                    <div>
                        <h1 style={topbarTitle}>Sistema de Facturación</h1>
                        <p style={topbarSubtitle}>
                            Gestión de productos, clientes y ventas
                        </p>
                    </div>
                </header>

                <section>{children}</section>
            </main>
        </div>
    );
}

const layout = {
    display: "flex",
    minHeight: "100vh",
    backgroundColor: "#f3f4f6",
};

const sidebar = {
    width: "260px",
    backgroundColor: "#111827",
    color: "white",
    display: "flex",
    flexDirection: "column",
    padding: "24px 18px",
    boxSizing: "border-box",
};

const brand = {
    marginBottom: "28px",
    paddingBottom: "20px",
    borderBottom: "1px solid rgba(255,255,255,0.12)",
};

const brandTitle = {
    margin: 0,
    fontSize: "28px",
    fontWeight: "bold",
};

const brandSubtitle = {
    margin: "8px 0 0 0",
    color: "#9ca3af",
    fontSize: "14px",
};

const nav = {
    display: "flex",
    flexDirection: "column",
    gap: "10px",
};

const botonNav = {
    backgroundColor: "transparent",
    color: "#e5e7eb",
    border: "none",
    padding: "12px 14px",
    borderRadius: "10px",
    textAlign: "left",
    cursor: "pointer",
    fontWeight: "bold",
    fontSize: "15px",
};

const botonActivo = {
    backgroundColor: "#2563eb",
    color: "white",
    border: "none",
    padding: "12px 14px",
    borderRadius: "10px",
    textAlign: "left",
    cursor: "pointer",
    fontWeight: "bold",
    fontSize: "15px",
};

const mainContent = {
    flex: 1,
    padding: "24px",
    boxSizing: "border-box",
};

const topbar = {
    backgroundColor: "white",
    borderRadius: "14px",
    padding: "18px 22px",
    marginBottom: "24px",
    boxShadow: "0 6px 18px rgba(0,0,0,0.06)",
    border: "1px solid #e5e7eb",
};

const topbarTitle = {
    margin: 0,
    fontSize: "26px",
};

const topbarSubtitle = {
    margin: "6px 0 0 0",
    color: "#6b7280",
    fontSize: "14px",
};

export default AdminLayout;