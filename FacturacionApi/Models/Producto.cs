using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace FacturacionApi.Models
{
    [Table("Productos")]
    public class Producto
    {
        [Key]
        public int Codigo { get; set; }

        [Required]
        [MaxLength(100)]
        public string Nombre { get; set; } = string.Empty;

        [Required]
        [MaxLength(50)]
        public string Categoria { get; set; } = string.Empty;

        [Required]
        [MaxLength(100)]
        public string Marca { get; set; } = string.Empty;

        [Required]
        public decimal Precio { get; set; }

        [Required]
        public int Stock { get; set; }

        [Required]
        [MaxLength(20)]
        public string Estado { get; set; } = string.Empty;
    }
}