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

    public class CrearDetalleVentaRequest
    {
        [Required]
        public int ProductoCodigo { get; set; }

        [Required]
        [Range(1, int.MaxValue)]
        public int Cantidad { get; set; }
    }
}