using FacturacionApi.Data;
using FacturacionApi.DTOs;
using FacturacionApi.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace FacturacionApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class VentasController : ControllerBase
    {
        private readonly FacturacionDbContext _context;

        public VentasController(FacturacionDbContext context)
        {
            _context = context;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<object>>> ObtenerTodas()
        {
            var ventas = await _context.Ventas
                .Include(v => v.Detalles)
                .OrderByDescending(v => v.Numero)
                .Select(v => new
                {
                    v.Numero,
                    v.Fecha,
                    v.ClienteId,
                    v.Subtotal,
                    v.Total,
                    v.Estado,
                    Detalles = v.Detalles.Select(d => new
                    {
                        d.Id,
                        d.ProductoCodigo,
                        d.Cantidad,
                        d.PrecioUnitario,
                        d.Subtotal
                    })
                })
                .ToListAsync();

            return Ok(ventas);
        }

        [HttpGet("{numero}")]
        public async Task<ActionResult<object>> ObtenerPorNumero(int numero)
        {
            var venta = await _context.Ventas
                .Include(v => v.Detalles)
                .Where(v => v.Numero == numero)
                .Select(v => new
                {
                    v.Numero,
                    v.Fecha,
                    v.ClienteId,
                    v.Subtotal,
                    v.Total,
                    v.Estado,
                    Detalles = v.Detalles.Select(d => new
                    {
                        d.Id,
                        d.ProductoCodigo,
                        d.Cantidad,
                        d.PrecioUnitario,
                        d.Subtotal
                    })
                })
                .FirstOrDefaultAsync();

            if (venta == null)
            {
                return NotFound();
            }

            return Ok(venta);
        }

        [HttpPost]
        public async Task<ActionResult> Crear([FromBody] CrearVentaRequest request)
        {
            if (request == null || request.Detalles == null || request.Detalles.Count == 0)
            {
                return BadRequest("La venta debe incluir al menos un producto.");
            }

            var cliente = await _context.Clientes.FindAsync(request.ClienteId);

            if (cliente == null)
            {
                return BadRequest("El cliente no existe.");
            }

            var codigos = request.Detalles
                .Select(d => d.ProductoCodigo)
                .Distinct()
                .ToList();

            var productos = await _context.Productos
                .Where(p => codigos.Contains(p.Codigo))
                .ToListAsync();

            if (productos.Count != codigos.Count)
            {
                return BadRequest("Uno o más productos no existen.");
            }

            foreach (var item in request.Detalles)
            {
                if (item.Cantidad <= 0)
                {
                    return BadRequest("Todas las cantidades deben ser mayores a cero.");
                }

                var producto = productos.First(p => p.Codigo == item.ProductoCodigo);

                if (!string.Equals(producto.Estado, "Activo", StringComparison.OrdinalIgnoreCase))
                {
                    return BadRequest($"El producto con código {producto.Codigo} no está activo.");
                }

                if (producto.Stock < item.Cantidad)
                {
                    return BadRequest($"Stock insuficiente para el producto con código {producto.Codigo}.");
                }
            }

            await using var transaction = await _context.Database.BeginTransactionAsync();

            try
            {
                var venta = new Venta
                {
                    ClienteId = request.ClienteId,
                    Fecha = DateTime.UtcNow,
                    Estado = "Registrada",
                    Subtotal = 0,
                    Total = 0
                };

                _context.Ventas.Add(venta);
                await _context.SaveChangesAsync();

                decimal subtotalVenta = 0;

                foreach (var item in request.Detalles)
                {
                    var producto = productos.First(p => p.Codigo == item.ProductoCodigo);
                    decimal precioUnitario = producto.Precio;
                    decimal subtotalLinea = precioUnitario * item.Cantidad;

                    var detalle = new DetalleVenta
                    {
                        VentaNumero = venta.Numero,
                        ProductoCodigo = producto.Codigo,
                        Cantidad = item.Cantidad,
                        PrecioUnitario = precioUnitario,
                        Subtotal = subtotalLinea
                    };

                    _context.DetalleVenta.Add(detalle);

                    producto.Stock -= item.Cantidad;
                    subtotalVenta += subtotalLinea;
                }

                venta.Subtotal = subtotalVenta;
                venta.Total = subtotalVenta;

                await _context.SaveChangesAsync();
                await transaction.CommitAsync();

                return CreatedAtAction(
                    nameof(ObtenerPorNumero),
                    new { numero = venta.Numero },
                    new
                    {
                        venta.Numero,
                        venta.Fecha,
                        venta.ClienteId,
                        venta.Subtotal,
                        venta.Total,
                        venta.Estado
                    });
            }
            catch
            {
                await transaction.RollbackAsync();
                throw;
            }
        }
    }
}