#include "sigam.h"

#include <ctype.h>  // permite usar caracteres; convertir letras y comprobar si son letras o numeros.
#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien
#include <windows.h>  //lo usamos para esperar x cantidad de tiempo
#include <conio.h>    //permite utilizar getch(),que captura una tecla sin mostrarla en pantalla.

void mostrarBienvenida() {
  printf("====================================================\n");
  printf("                BIENVENIDO/A A SIGAM                \n");
  printf("====================================================\n");
  printf("Sistema de Gestion y Asignacion de Auxilio Mecanico \n");
  printf("....................................................\n");
}

void mostrarSolicitudAuxilio(void) {
  printf(
      "===================================================================\n");
  printf(
      "                         Solicitar auxilio                         \n");
  printf(
      "===================================================================\n");
  printf(
      "A continuacion, seleccione y complete las opciones de la solicitud     "
      "\n");
  printf(
      "....................................................................\n");
}

/*------------------------------------------------------------------------*/
/*                               lOGIN USUARIOS                           */
/*------------------------------------------------------------------------*/
void inicioSesion(char usuario[], char password[]) {
  printf("Ingrese su usuario: ");
  scanf("%s", usuario);

  printf("Ingrese su clave: ");
   ingresarClave(password, TEXTO);
}
// Aclaraciones:La funcion devuelve una constante que identifica el perfil.
//  PERFIL_CLIENTE, PERFIL_CHOFER, PERFIL_ADMIN o PERFIL_SOPORTE.
// strcmp() compara dos cadenas y devuelve 0 cuando son iguales.
//  usuario y password se comparan sin [] porque eso devolveria una sola letra
//  justamente por lo dicho arriba

int ingresarClave(char clave[], int tam) {
    int c;
    int i = 0;

    do {
        c = getch();
        if (c == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else if (c != 13 && i < tam - 1) {
            clave[i] = c;
            i++;
            printf("*");
        }
    } while (c != 13);
    clave[i] = '\0';
    printf("\n");
    return 0;
}

int validaLogin(char usuario[], char password[], perfil_t perfiles[],
                int* usuarioLogueado) {
  int i = 0;
  int tipoUsuario = LOGIN_INCORRECTO;

  while (i < CANT_PERFILES && tipoUsuario == LOGIN_INCORRECTO) {
    // Si encuentra el usuario y la contraseña correctos:
    if (strcmp(usuario, perfiles[i].usuario) == 0 &&
        strcmp(password, perfiles[i].clave) == 0) {
      // Acá adentro recién chequeamos si está Activo o Suspendido
      if (strcmp(perfiles[i].estado, "Activo") == 0) {
        *usuarioLogueado = i;
        tipoUsuario = perfiles[i].tipoPerfil;  // Devuelve el perfil correspondiente
      } else {
        tipoUsuario = LOGIN_SUSPENDIDO;  // Solo si coincide pero está suspendido
      }
    }
    i++;
  }
  return tipoUsuario;
}



/* Motor de búsqueda */
int motorDeBusqueda(chofer_t choferes[], solicitudes_t solicitudes[],
                    int indiceSolicitud) {
  int buscaChofer = -1;
  int i;
  for (i = 0; i < CANT_CHOFERES; i++) {
    if (buscaChofer == -1) {
      if (choferes[i].estado == DISPONIBLE) {
        if (choferes[i].ubicacion ==
            solicitudes[indiceSolicitud].ubicacion) {
          if (choferes[i].tipoGrua ==
              solicitudes[indiceSolicitud].tipoVehiculo) {
            buscaChofer = i;
          }
        }
      }
    }
  }
  return buscaChofer;
}

/*Motor de asignación*/
void motorDeAsignacion(chofer_t choferes[], solicitudes_t solicitudes[],
                       int choferEncontrado, int indiceSolicitud) {
  if (choferEncontrado != -1) {
    choferes[choferEncontrado].solicitudAsignada = indiceSolicitud;
    choferes[choferEncontrado].estado = OCUPADO;
    solicitudes[indiceSolicitud].estado = CHOFER_ASIGNADO;
    solicitudes[indiceSolicitud].choferAsignado = choferEncontrado;
    printf("Su solicitud ha sido confirmada\n");
    printf("Estado: chofer asignado\n");
  } else {
    solicitudes[indiceSolicitud].estado = SOLICITUD_PENDIENTE;
    solicitudes[indiceSolicitud].choferAsignado = -1;
    printf("Su solicitud ha sido registrada\n");
    printf("Estado: solicitud pendiente\n");
  }
}

int buscarSolicitudPendiente(chofer_t choferes[], solicitudes_t solicitudes[], int cantSolicitudes, int choferLogueado){
  int solicitudEncontrada = -1;
  int mayorPuntaje = -1;
  int i;

  for (i = 0; i<cantSolicitudes; i++){
    if (solicitudes[i].estado == SOLICITUD_PENDIENTE){
      if (choferes[choferLogueado].ubicacion == solicitudes[i].ubicacion){
        if(solicitudes[i].prioridad > mayorPuntaje){
          mayorPuntaje= solicitudes[i].prioridad;
          solicitudEncontrada= i;
        }
      }
    }
  }
  return solicitudEncontrada;
}

/*Calculo de prioridad según cuestionario*/
int calcularPrioridad(void) {
  int opcionVia, opcionPeligro, opcionObstruye, opcionDetenido, puntaje,
      prioridad;
  puntaje = 0;
  do {
    mostrarSolicitudAuxilio();
    printf("En que tipo de via se encuentra?: \n");
    printf("1 - Autopista\n");
    printf("2 - Ruta\n");
    printf("3 - Calle\n");
    printf("Opcion: ");
    scanf("%d", &opcionVia);
    switch (opcionVia) {
      case 1:
        puntaje = puntaje + 3;
        break;
      case 2:
        puntaje = puntaje + 1;
        break;
      case 3:
        puntaje;
        break;
      default:
        printf("Opcion incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
        break;
    }
  } while (opcionVia < 1 || opcionVia > 3);
  system("cls");

  do {
    mostrarSolicitudAuxilio();
    printf("Se encuentra en una zona peligrosa?: \n");
    printf("1 - Si\n");
    printf("2 - No\n");
    printf("Opcion: ");
    scanf("%d", &opcionPeligro);
    switch (opcionPeligro) {
      case 1:
        puntaje = puntaje + 3;
        break;
      case 2:
        puntaje;
        break;
      default:
        printf("Opcion incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
        break;
    }
  } while (opcionPeligro < 1 || opcionPeligro > 2);
  system("cls");

  do {
    mostrarSolicitudAuxilio();
    printf("Se encuentra obstruyendo la circulacion?: \n");
    printf("1 - Si\n");
    printf("2 - No\n");
    printf("Opcion: ");
    scanf("%d", &opcionObstruye);
    switch (opcionObstruye) {
      case 1:
        puntaje = puntaje + 2;
        break;
      case 2:
        puntaje;
        break;
      default:
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
        break;
    }
  } while (opcionObstruye < 1 || opcionObstruye > 2);
  system("cls");

  do {
    mostrarSolicitudAuxilio();
    printf("Su vehiculo esta totalmente detenido?: \n");
    printf("1 - Si\n");
    printf("2 - No\n");
    printf("Opcion: ");
    scanf("%d", &opcionDetenido);
    switch (opcionDetenido) {
      case 1:
        puntaje = puntaje + 2;
        break;
      case 2:
        puntaje;
        break;
      default:
        printf("Opcion incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
        break;
    }
  } while (opcionDetenido < 1 || opcionDetenido > 2);
  system("cls");
  prioridad = puntaje;
  return prioridad;
}

void mostrarPrioridad(int prioridad) {
  if (prioridad <= 2) {
    printf("Baja\n");
  } else if (prioridad < 6) {
    printf("Media\n");
  } else {
    printf("Alta\n");
  }
}

/*Validación para la patente de la opción Solicitar Auxilio*/
bool validarPatente(char patente[]) {
  int i;
  // valida que el usuario haya ingresado exactamente 6 caracteres
  if (strlen(patente) != 6) {  // strlen = tring length = longitud de la
                               // cadena, cuenta cuantos caracteres hay
    return false;
  }
  /*unsigned char: evita errores por caract. especiales */
  // Si el usuario ingresa mayusculas, lo pasa a minusculas. Evita error por
  // patente invalida
  for (i = 0; i < 6; i++) {
    patente[i] = tolower(
        (unsigned char)patente[i]);  // tolower = to lower case = a minuscula
  }
  // valida que el usuario haya ingresado 3 letras (entre a y z)
  for (i = 0; i < 3; i++) {  // recorre las 3 primeras posiciones
    if (!isalpha((unsigned char)
                     patente[i])) {  // isalpha = is alphabetical = es una letra
      return false;
    }
  }
  // valida que el usuario haya ingresado 3 numeros (entre 0 y 9)
  for (i = 3; i < 6; i++) {  // recorre las 3 ultimas posiciones
    if (!isdigit(
            (unsigned char)patente[i])) {  // isdigit = is digit = es un dígito
      return false;
    }
  }
  return true;
}


