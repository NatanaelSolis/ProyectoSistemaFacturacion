const API_URL = "https://localhost:7016/api/Ventas";

export async function obtenerVentas() {
    const response = await fetch(API_URL);

    if (!response.ok) {
        throw new Error("No se pudieron obtener las ventas");
    }

    return await response.json();
}

export async function obtenerVentaPorNumero(numero) {
    const response = await fetch(`${API_URL}/${numero}`);

    if (!response.ok) {
        throw new Error("No se pudo obtener la venta");
    }

    return await response.json();
}

export async function crearVenta(venta) {
    const response = await fetch(API_URL, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(venta),
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo registrar la venta");
    }

    return await response.json();
}