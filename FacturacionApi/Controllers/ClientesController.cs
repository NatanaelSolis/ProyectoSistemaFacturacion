using System.Globalization;
using FacturacionApi.Data;
using FacturacionApi.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace FacturacionApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ClientesController : ControllerBase
    {
        private readonly FacturacionDbContext _context;

        public ClientesController(FacturacionDbContext context)
        {
            _context = context;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<Cliente>>> ObtenerTodos()
        {
            var clientes = await _context.Clientes
                .OrderBy(c => c.Id)
                .ToListAsync();

            return Ok(clientes);
        }

        [HttpGet("carga-inicial")]
        public async Task<ActionResult> ObtenerCargaInicial()
        {
            var clientes = await _context.Clientes
                .OrderBy(c => c.Id)
                .ToListAsync();

            var lineas = clientes.Select(c =>
                string.Join(";",
                    c.Id.ToString(CultureInfo.InvariantCulture),
                    LimpiarCampo(c.Nombre),
                    LimpiarCampo(c.Correo),
                    LimpiarCampo(c.Telefono),
                    LimpiarCampo(c.Direccion)
                ));

            var contenido = string.Join(Environment.NewLine, lineas);

            return Content(contenido, "text/plain");
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<Cliente>> ObtenerPorId(int id)
        {
            var cliente = await _context.Clientes.FindAsync(id);

            if (cliente == null)
            {
                return NotFound();
            }

            return Ok(cliente);
        }

        [HttpPost]
        public async Task<ActionResult> Insertar([FromBody] Cliente cliente)
        {
            var existente = await _context.Clientes.FindAsync(cliente.Id);

            if (existente != null)
            {
                return BadRequest("Ya existe un cliente con ese ID.");
            }

            _context.Clientes.Add(cliente);
            await _context.SaveChangesAsync();

            return CreatedAtAction(nameof(ObtenerPorId), new { id = cliente.Id }, cliente);
        }

        [HttpDelete("{id}")]
        public async Task<ActionResult> Eliminar(int id)
        {
            var cliente = await _context.Clientes.FindAsync(id);

            if (cliente == null)
            {
                return NotFound();
            }

            _context.Clientes.Remove(cliente);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> Actualizar(int id, Cliente clienteActualizado)
        {
            if (id != clienteActualizado.Id)
            {
                return BadRequest("El ID no coincide.");
            }

            var clienteExistente = await _context.Clientes.FindAsync(id);

            if (clienteExistente == null)
            {
                return NotFound("Cliente no encontrado.");
            }

            clienteExistente.Nombre = clienteActualizado.Nombre;
            clienteExistente.Correo = clienteActualizado.Correo;
            clienteExistente.Telefono = clienteActualizado.Telefono;
            clienteExistente.Direccion = clienteActualizado.Direccion;

            await _context.SaveChangesAsync();

            return Ok(new { mensaje = "Cliente actualizado correctamente." });
        }


        private static string LimpiarCampo(string valor)
        {
            return (valor ?? string.Empty)
                .Replace(";", ",")
                .Replace("\r", " ")
                .Replace("\n", " ");
        }
    }
}