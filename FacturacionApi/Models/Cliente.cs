using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace FacturacionApi.Models
{
    [Table("Clientes")]
    public class Cliente
    {
        [Key]
        public int Id { get; set; }

        [Required]
        [MaxLength(100)]
        public string Nombre { get; set; } = string.Empty;

        [Required]
        [MaxLength(150)]
        [EmailAddress]
        public string Correo { get; set; } = string.Empty;

        [Required]
        [MaxLength(30)]
        public string Telefono { get; set; } = string.Empty;

        [Required]
        [MaxLength(250)]
        public string Direccion { get; set; } = string.Empty;
    }
}