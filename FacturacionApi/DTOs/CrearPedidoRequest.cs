using System.ComponentModel.DataAnnotations;

namespace FacturacionApi.DTOs
{
    public class CrearPedidoRequest
    {
        [Required]
        public int ClienteId { get; set; }

        [Required]
        [MinLength(1)]
        public List<CrearDetallePedidoRequest> Detalles { get; set; } = new();
    }
}