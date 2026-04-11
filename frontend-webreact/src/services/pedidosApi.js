const API_URL = "https://localhost:7016/api/Pedidos";

export async function obtenerPedidos() {
    const response = await fetch(API_URL);

    if (!response.ok) {
        throw new Error("No se pudieron obtener los pedidos");
    }

    return await response.json();
}

export async function obtenerSiguientePedido() {
    const response = await fetch(`${API_URL}/siguiente`);

    if (!response.ok) {
        throw new Error("No se pudo obtener el siguiente pedido");
    }

    return await response.json();
}

export async function crearPedido(pedido) {
    const response = await fetch(API_URL, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(pedido),
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo crear el pedido");
    }

    return await response.json();
}

export async function atenderPedido() {
    const response = await fetch(`${API_URL}/atender`, {
        method: "POST",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo atender el pedido");
    }

    return await response.json();
}