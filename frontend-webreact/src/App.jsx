import { useState } from "react";
import AdminLayout from "./components/AdminLayout";
import DashboardPage from "./pages/DashboardPage";
import ProductosPage from "./pages/ProductosPage";
import ClientesPage from "./pages/ClientesPage";
import VentasPage from "./pages/VentasPage";
import PedidosPage from "./pages/PedidosPage";

function App() {
    const [modulo, setModulo] = useState("dashboard");

    function renderModulo() {
        switch (modulo) {
            case "dashboard":
                return <DashboardPage />;
            case "productos":
                return <ProductosPage />;
            case "clientes":
                return <ClientesPage />;
            case "ventas":
                return <VentasPage />;
            case "pedidos":
                return <PedidosPage />;
            default:
                return <DashboardPage />;
        }
    }

    return (
        <AdminLayout modulo={modulo} setModulo={setModulo}>
            {renderModulo()}
        </AdminLayout>
    );
}

export default App;