using System.ComponentModel.DataAnnotations;

namespace FacturacionApi.DTOs
{
    public class CrearDetalleVentaRequest
    {
        [Required]
        public int ProductoCodigo { get; set; }

        [Required]
        [Range(1, int.MaxValue)]
        public int Cantidad { get; set; }

        public decimal? PrecioUnitario { get; set; }
    }
}