using FacturacionApi.Data;
using FacturacionApi.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace FacturacionApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ProductosController : ControllerBase
    {
        private readonly FacturacionDbContext _context;

        public ProductosController(FacturacionDbContext context)
        {
            _context = context;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<Producto>>> ObtenerTodos()
        {
            var productos = await _context.Productos
                .OrderBy(p => p.Codigo)
                .ToListAsync();

            return Ok(productos);
        }

        [HttpGet("{codigo}")]
        public async Task<ActionResult<Producto>> ObtenerPorCodigo(int codigo)
        {
            var producto = await _context.Productos.FindAsync(codigo);

            if (producto == null)
            {
                return NotFound();
            }

            return Ok(producto);
        }

        [HttpPost]
        public async Task<ActionResult> Insertar([FromBody] Producto producto)
        {
            var existente = await _context.Productos.FindAsync(producto.Codigo);

            if (existente != null)
            {
                return BadRequest("Ya existe un producto con ese codigo.");
            }

            _context.Productos.Add(producto);
            await _context.SaveChangesAsync();

            return CreatedAtAction(nameof(ObtenerPorCodigo), new { codigo = producto.Codigo }, producto);
        }

        [HttpDelete("{codigo}")]
        public async Task<ActionResult> Eliminar(int codigo)
        {
            var producto = await _context.Productos.FindAsync(codigo);

            if (producto == null)
            {
                return NotFound();
            }

            _context.Productos.Remove(producto);
            await _context.SaveChangesAsync();

            return NoContent();
        }
    }
}