const API_URL = "https://localhost:7016/api/Productos";

export async function obtenerProductos() {
    const response = await fetch(API_URL);

    if (!response.ok) {
        throw new Error("No se pudieron obtener los productos");
    }

    return await response.json();
}

export async function crearProducto(producto) {
    const response = await fetch(API_URL, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(producto),
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo crear el producto");
    }

    return await response.json().catch(() => null);
}

export async function actualizarProducto(codigo, producto) {
    const response = await fetch(`${API_URL}/${codigo}`, {
        method: "PUT",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(producto),
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo actualizar el producto");
    }

    return await response.json().catch(() => null);
}

export async function desactivarProducto(codigo) {
    const response = await fetch(`${API_URL}/${codigo}`, {
        method: "DELETE",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo desactivar el producto");
    }

    return true;
}

export async function activarProducto(codigo) {
    const response = await fetch(`${API_URL}/${codigo}/activar`, {
        method: "PUT",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo activar el producto");
    }

    return true;
}