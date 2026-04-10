#pragma once

#include <string>

class PedidoPendiente
{
private:
    int numero;
    int clienteId;
    std::string fecha;
    std::string estado;
    double total;

public:
    PedidoPendiente();
    PedidoPendiente(int numero, int clienteId, const std::string& fecha, const std::string& estado, double total);

    int getNumero() const;
    int getClienteId() const;
    std::string getFecha() const;
    std::string getEstado() const;
    double getTotal() const;

    void setNumero(int numero);
    void setClienteId(int clienteId);
    void setFecha(const std::string& fecha);
    void setEstado(const std::string& estado);
    void setTotal(double total);

    void mostrar() const;
};