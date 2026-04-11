const API_URL = "https://localhost:7016/api/Clientes";

export async function obtenerClientes() {
    const response = await fetch(API_URL);

    if (!response.ok) {
        throw new Error("No se pudieron obtener los clientes");
    }

    return await response.json();
}

export async function crearCliente(cliente) {
    const response = await fetch(API_URL, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(cliente),
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo crear el cliente");
    }

    return await response.json().catch(() => null);
}

export async function actualizarCliente(id, cliente) {
    const response = await fetch(`${API_URL}/${id}`, {
        method: "PUT",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(cliente),
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo actualizar el cliente");
    }

    return await response.json().catch(() => null);
}

export async function eliminarCliente(id) {
    const response = await fetch(`${API_URL}/${id}`, {
        method: "DELETE",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || "No se pudo eliminar el cliente");
    }

    return true;
}