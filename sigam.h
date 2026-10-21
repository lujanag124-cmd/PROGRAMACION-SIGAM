#ifndef SIGAM_H
#define SIGAM_H

#include <stdbool.h>

// Constantes
#define CANT_PERFILES 8
#define CANT_CHOFERES 3
#define CANT_SOLICITUDES 10
#define TEXTO 30
#define DISPONIBLE 1
#define OCUPADO 2
#define NO_DISPONIBLE 3
#define GRUA_MOTO 1
#define GRUA_AUTO 2
#define GRUA_CAMIONETA 3
#define SIN_SOLICITUD 0
#define SOLICITUD_PENDIENTE 1
#define CHOFER_ASIGNADO 2
#define VIAJE_INICIADO 3
#define VIAJE_FINALIZADO 4
#define LOGIN_INCORRECTO 0
#define PERFIL_CLIENTE 1
#define PERFIL_CHOFER 2
#define PERFIL_ADMIN 3
#define PERFIL_SOPORTE 4
#define VEHICULO_MOTO 1
#define VEHICULO_AUTO 2
#define VEHICULO_CAMIONETA 3
#define MERLO 1
#define ITUZAINGO 2
#define MORON 3
#define CANT_POSIBILIDADES 3
#define CANT_MAX_PUNT 10
#define CANT_RECLAMOS 10
#define LOGIN_SUSPENDIDO -1  // identifica si es activo - suspendido

/* Estrucuturas de los perfiles generales */
typedef struct {
  int tipoPerfil;
  char nombre[TEXTO];
  char apellido[TEXTO];
  char usuario[TEXTO];
  char clave[TEXTO];
  char estado[TEXTO];
} perfil_t;

/* Estrucutura de los choferes */
typedef struct {
  int idPerfil;
  int estado;
  int ubicacion;
  int tipoGrua;
  int solicitudAsignada;
} chofer_t;

/* Estructura de la solicitud */
typedef struct {
  int idCliente;
  int ubicacion;
  int tipoVehiculo;
  int situacion;
  char patente[TEXTO];
  int estado;
  int choferAsignado;
  int prioridad;
} solicitudes_t;

/*SECCIÓN SOPORTE - Estructura para los reclamos */
typedef struct {
  char titulo[TEXTO];
  char acusado[TEXTO];
  char mensaje[TEXTO];
  char tipoUsuario[TEXTO];  // "Chofer" o "Cliente"
  char estado[TEXTO];       // "Pendiente" o "Validado"
  char respuesta[TEXTO];    // Resolución del soporte
} reclamo_t;

/* Declaracion de los prototipos */
void mostrarBienvenida(void);
void inicioSesion(char usuario[], char password[]);
int validaLogin(char usuario[], char password[], perfil_t perfiles[],
                int* usuarioLogueado);
void menuCliente(int usuarioLogueado, solicitudes_t solicitudes[],
                 chofer_t choferes[], int* cantSolicitudes, int* cantCanceladas,
                 char ubicaciones[][TEXTO], char situaciones[][TEXTO],
                 char tiposVehiculos[][TEXTO], reclamo_t reclamos[],
                 int* cantReclamos, perfil_t perfiles[]);
void menuChofer(int usuarioLogueado, chofer_t choferes[],
                char ubicaciones[][TEXTO], char tiposGrua[][TEXTO],
                reclamo_t reclamos[], int* cantReclamos, perfil_t perfiles[]);
void menuAdmin();
void menuSoporte();
bool registrarSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],
                        int* cantSolicitudes, int* cantCanceladas,
                        char ubicaciones[][TEXTO], char situaciones[][TEXTO],
                        char tiposVehiculos[][TEXTO]);
int calcularPrioridad(void);
void mostrarPrioridad(int prioridad);
void consultaSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],
                       int cantSolicitudes, char ubicacion[][TEXTO],
                       char tiposVehiculos[][TEXTO], char situacion[][TEXTO]);
void estadoActual(int usuarioLogueado, chofer_t choferes[],
                  char ubicacion[][TEXTO], char tipoGrua[][TEXTO]);
int motorDeBusqueda(chofer_t choferes[], solicitudes_t solicitudes[],
                    int cantSolicitudes);
void motorDeAsignacion(chofer_t choferes[], solicitudes_t solicitudes[],
                       int choferEncontrado, int indiceSolicitud);
void mostrarSolicitudAuxilio(void);
bool validarPatente(char patente[]);
void menuSoporte(int usuarioLogueado, reclamo_t reclamos[], int* cantReclamos,
                 perfil_t perfiles[]);
void contactarSoporte(char tipoUsuarioActual[], reclamo_t reclamos[],
                      int* cantReclamos);
void verReclamosPendientes(reclamo_t reclamos[], int cantReclamos);
void gestionarEstadoUsuarios(perfil_t perfiles[]);
void verHistorialReclamos(reclamo_t reclamos[], int cantReclamos);
void verReclamosPendientes(reclamo_t reclamos[], int cantReclamos);
void verHistorialReclamos(reclamo_t reclamos[], int cantReclamos);
void hacerReclamo(int usuarioLogueado, reclamo_t reclamos[], int* cantReclamos,
                  perfil_t perfiles[]);
#endif