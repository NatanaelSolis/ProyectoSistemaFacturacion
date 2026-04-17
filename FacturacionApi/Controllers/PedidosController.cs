using FacturacionApi.Data;
using FacturacionApi.DTOs;
using FacturacionApi.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace FacturacionApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class PedidosController : ControllerBase
    {
        private readonly FacturacionDbContext _context;

        public PedidosController(FacturacionDbContext context)
        {
            _context = context;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<object>>> ObtenerTodos()
        {
            var pedidos = await _context.Pedidos
                .Include(p => p.Detalles)
                .OrderBy(p => p.Numero)
                .Select(p => new
                {
                    p.Numero,
                    p.Fecha,
                    p.ClienteId,
                    p.Subtotal,
                    p.IVA,
                    p.Total,
                    p.Estado,
                    Detalles = p.Detalles.Select(d => new
                    {
                        d.Id,
                        d.ProductoCodigo,
                        d.Cantidad,
                        d.PrecioUnitario,
                        d.Subtotal
                    })
                })
                .ToListAsync();

            return Ok(pedidos);
        }

        [HttpGet("{numero}")]
        public async Task<ActionResult<object>> ObtenerPorNumero(int numero)
        {
            var pedido = await _context.Pedidos
                .Include(p => p.Detalles)
                .Where(p => p.Numero == numero)
                .Select(p => new
                {
                    p.Numero,
                    p.Fecha,
                    p.ClienteId,
                    p.Subtotal,
                    p.IVA,
                    p.Total,
                    p.Estado,
                    Detalles = p.Detalles.Select(d => new
                    {
                        d.Id,
                        d.ProductoCodigo,
                        d.Cantidad,
                        d.PrecioUnitario,
                        d.Subtotal
                    })
                })
                .FirstOrDefaultAsync();

            if (pedido == null)
            {
                return NotFound("Pedido no encontrado.");
            }

            return Ok(pedido);
        }

        [HttpGet("siguiente")]
        public async Task<ActionResult<object>> ObtenerSiguiente()
        {
            var pedido = await _context.Pedidos
                .Include(p => p.Detalles)
                .Where(p => p.Estado == "Pendiente")
                .OrderBy(p => p.Numero)
                .Select(p => new
                {
                    p.Numero,
                    p.Fecha,
                    p.ClienteId,
                    p.Subtotal,
                    p.IVA,
                    p.Total,
                    p.Estado,
                    Detalles = p.Detalles.Select(d => new
                    {
                        d.Id,
                        d.ProductoCodigo,
                        d.Cantidad,
                        d.PrecioUnitario,
                        d.Subtotal
                    })
                })
                .FirstOrDefaultAsync();

            if (pedido == null)
            {
                return NotFound("No hay pedidos pendientes.");
            }

            return Ok(pedido);
        }

        [HttpPost]
        public async Task<ActionResult> Crear([FromBody] CrearPedidoRequest request)
        {
            if (request == null || request.Detalles == null || request.Detalles.Count == 0)
            {
                return BadRequest("El pedido debe incluir al menos un producto.");
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
            }

            await using var transaction = await _context.Database.BeginTransactionAsync();

            try
            {
                var pedido = new Pedido
                {
                    ClienteId = request.ClienteId,
                    Fecha = DateTime.UtcNow,
                    Estado = "Pendiente",
                    Subtotal = 0,
                    IVA = 0,
                    Total = 0
                };

                _context.Pedidos.Add(pedido);
                await _context.SaveChangesAsync();

                decimal totalPedidoConIVA = 0m;

                foreach (var item in request.Detalles)
                {
                    var producto = productos.First(p => p.Codigo == item.ProductoCodigo);

                    decimal precioUnitarioConIVA = producto.Precio;
                    decimal subtotalLineaConIVA = precioUnitarioConIVA * item.Cantidad;

                    var detalle = new DetallePedido
                    {
                        PedidoNumero = pedido.Numero,
                        ProductoCodigo = producto.Codigo,
                        Cantidad = item.Cantidad,
                        PrecioUnitario = precioUnitarioConIVA,
                        Subtotal = subtotalLineaConIVA
                    };

                    _context.DetallePedido.Add(detalle);
                    totalPedidoConIVA += subtotalLineaConIVA;
                }

                decimal subtotalSinIVA = Math.Round(totalPedidoConIVA / 1.13m, 2, MidpointRounding.AwayFromZero);
                decimal ivaIncluido = Math.Round(totalPedidoConIVA - subtotalSinIVA, 2, MidpointRounding.AwayFromZero);

                pedido.Subtotal = subtotalSinIVA;
                pedido.IVA = ivaIncluido;
                pedido.Total = totalPedidoConIVA;

                await _context.SaveChangesAsync();
                await transaction.CommitAsync();

                return CreatedAtAction(
                    nameof(ObtenerPorNumero),
                    new { numero = pedido.Numero },
                    new
                    {
                        pedido.Numero,
                        pedido.Fecha,
                        pedido.ClienteId,
                        pedido.Subtotal,
                        pedido.IVA,
                        pedido.Total,
                        pedido.Estado
                    });
            }
            catch
            {
                await transaction.RollbackAsync();
                throw;
            }
        }

        [HttpPost("{numero}/atender")]
        public async Task<ActionResult> Atender(int numero)
        {
            var pedido = await _context.Pedidos
                .Include(p => p.Detalles)
                .FirstOrDefaultAsync(p => p.Numero == numero);

            if (pedido == null)
            {
                return NotFound("Pedido no encontrado.");
            }

            if (!string.Equals(pedido.Estado, "Pendiente", StringComparison.OrdinalIgnoreCase))
            {
                return BadRequest("El pedido ya fue procesado o no está pendiente.");
            }

            var codigos = pedido.Detalles
                .Select(d => d.ProductoCodigo)
                .Distinct()
                .ToList();

            var productos = await _context.Productos
                .Where(p => codigos.Contains(p.Codigo))
                .ToListAsync();

            foreach (var detalle in pedido.Detalles)
            {
                var producto = productos.FirstOrDefault(p => p.Codigo == detalle.ProductoCodigo);

                if (producto == null)
                {
                    return BadRequest($"No existe el producto con código {detalle.ProductoCodigo}.");
                }

                if (!string.Equals(producto.Estado, "Activo", StringComparison.OrdinalIgnoreCase))
                {
                    return BadRequest($"El producto con código {producto.Codigo} no está activo.");
                }

                if (producto.Stock < detalle.Cantidad)
                {
                    return BadRequest($"Stock insuficiente para el producto con código {producto.Codigo}.");
                }
            }

            await using var transaction = await _context.Database.BeginTransactionAsync();

            try
            {
                var venta = new Venta
                {
                    ClienteId = pedido.ClienteId,
                    Fecha = DateTime.UtcNow,
                    Estado = "Registrada",
                    Subtotal = pedido.Subtotal,
                    IVA = pedido.IVA,
                    Total = pedido.Total
                };

                _context.Ventas.Add(venta);
                await _context.SaveChangesAsync();

                foreach (var detallePedido in pedido.Detalles)
                {
                    var producto = productos.First(p => p.Codigo == detallePedido.ProductoCodigo);

                    var detalleVenta = new DetalleVenta
                    {
                        VentaNumero = venta.Numero,
                        ProductoCodigo = detallePedido.ProductoCodigo,
                        Cantidad = detallePedido.Cantidad,
                        PrecioUnitario = detallePedido.PrecioUnitario,
                        Subtotal = detallePedido.Subtotal
                    };

                    _context.DetalleVenta.Add(detalleVenta);
                    producto.Stock -= detallePedido.Cantidad;
                }

                pedido.Estado = "Procesado";

                await _context.SaveChangesAsync();
                await transaction.CommitAsync();

                return Ok(new
                {
                    mensaje = "Pedido atendido y convertido en venta correctamente.",
                    pedidoNumero = pedido.Numero,
                    ventaNumero = venta.Numero
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