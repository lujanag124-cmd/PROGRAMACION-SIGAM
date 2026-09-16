#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien

#define CTEMP 5 /*ejemplo de variable fija*/

int main() {
  // Variables generales
  int usuario, password, opcionUsuario, choferEncontrado;
  bool loginActivo, solicitudConfirmada;
  char valorUsuario;  // Guarda la info de validalogin

  // Vectores vacíos de solicitudes (se completan dinámicamente durante el
  // programa)
  int choferesAsignados[10];
  const char* solicitudUbicaciones[10];
  const char* solicitudTipoVehiculos[10];
  const char* solicitudSituaciones[10];
  const char* patentes[10];

  // Opciones generales del sistema
  const char* ubicaciones[3] = {"Merlo", "Ituzaingo", "Moron"};
  const char* tipoVehiculos[3] = {"Moto", "Auto", "Camioneta"};
  const char* situaciones[3] = {"Pinchadura ", "Falla mecanica ",
                                "Accidente/Choque "};

  // Datos de los choferes
  const char* nombresChoferes[3] = {"Ruben", "Carlos", "Tomi"};
  const char* usuariosChoferes[3] = {"chofer1", "chofer2", "chofer3"};
  const char* clavesChoferes[3] = {"chofer123", "chofer123", "chofer123"};
  const char* estadosChoferes[3] = {"Disponible", "Ocupado", "Disponible"};
  const char* tiposGrua[3] = {"Auto", "Moto", "Camioneta"};
  const char* localidadesChoferes[3] = {"Ituzaingo", "Moron", "Merlo"};

  // Estados posibles
  const char* estadosDisponibles[3] = {"Disponible", "Ocupado",
                                       "No disponible"};

  int cantSolicitudes, cantCanceladas;
  cantSolicitudes = 0;
  cantSolicitudes = 0;

  while (true) {  // estre true es para funcione mientras la sesión siga activa
    valorUsuario = "Incorrecto";
    while (valorUsuario == "Incorrecto") {
      system("cls");  // Limpiar Pantalla
      printf("==========================================\n");
      printf("        BIENVENIDO/A A SIGAM              \n");
      printf("==========================================\n");

      inicioSesion(usuario, password);
      valorUsuario = validaLogin(usuario, password);

      if (valorUsuario == "Incorrecto") {
        printf(
            "Usuario o contraseña incorrecta. Por favor, intente nuevamente."
            "nuevamente.\n");
        Sleep(3000);    // Esperar 3.0 Segundos
        system("cls");  // Limpiar Pantalla
      }
    }

    // Al iniciar sesión, limpiará para mostrar correctamente los menús
    system("cls");  // Limpiar Pantalla

    loginActivo = true;
    while (loginActivo) {
      switch (valorUsuario) {
        case Chofer:
          /* code */
          break;

        default:
          break;
      }
    }
  }

  return 0;
}
