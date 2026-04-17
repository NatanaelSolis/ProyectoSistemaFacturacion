using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace FacturacionApi.Models
{
    [Table("DetallePedido")]
    public class DetallePedido
    {
        [Key]
        public int Id { get; set; }

        [Required]
        public int PedidoNumero { get; set; }

        [Required]
        public int ProductoCodigo { get; set; }

        [Required]
        public int Cantidad { get; set; }

        [Required]
        [Column(TypeName = "decimal(18,2)")]
        public decimal PrecioUnitario { get; set; }

        [Required]
        [Column(TypeName = "decimal(18,2)")]
        public decimal Subtotal { get; set; }

        [ForeignKey("PedidoNumero")]
        public Pedido? Pedido { get; set; }

        [ForeignKey("ProductoCodigo")]
        public Producto? Producto { get; set; }
    }
}