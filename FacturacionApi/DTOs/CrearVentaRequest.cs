using System.ComponentModel.DataAnnotations;

namespace FacturacionApi.DTOs
{
    public class CrearVentaRequest
    {
        [Required]
        public int ClienteId { get; set; }

        [Required]
        [MinLength(1)]
        public List<CrearDetalleVentaRequest> Detalles { get; set; } = new();
    }
}