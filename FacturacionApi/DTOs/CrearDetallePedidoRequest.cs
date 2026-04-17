using System.ComponentModel.DataAnnotations;

namespace FacturacionApi.DTOs
{
    public class CrearDetallePedidoRequest
    {
        [Required]
        public int ProductoCodigo { get; set; }

        [Required]
        [Range(1, int.MaxValue)]
        public int Cantidad { get; set; }
    }
}