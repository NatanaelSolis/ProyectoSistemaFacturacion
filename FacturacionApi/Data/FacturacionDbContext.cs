using FacturacionApi.Models;
using Microsoft.EntityFrameworkCore;

namespace FacturacionApi.Data
{
    public class FacturacionDbContext : DbContext
    {
        public FacturacionDbContext(DbContextOptions<FacturacionDbContext> options)
            : base(options)
        {
        }

        public DbSet<Producto> Productos { get; set; }
        public DbSet<Cliente> Clientes { get; set; }
    }
}