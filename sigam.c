#include "sigam.h"

#include <ctype.h>  // permite usar caracteres; convertir letras y comprobar si son letras o numeros.
#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien
#include <windows.h>  //lo usamos para esperar x cantidad de tiempo

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
  scanf("%s", password);
}
// Aclaraciones:La funcion devuelve una constante que identifica el perfil.
//  PERFIL_CLIENTE, PERFIL_CHOFER, PERFIL_ADMIN o PERFIL_SOPORTE.
// strcmp() compara dos cadenas y devuelve 0 cuando son iguales.
//  usuario y password se comparan sin [] porque eso devolveria una sola letra
//  justamente por lo dicho arriba

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
        tipoUsuario =
            perfiles[i].tipoPerfil;  // Devuelve el perfil correspondiente
      } else {
        tipoUsuario =
            LOGIN_SUSPENDIDO;  // Solo si coincide pero está suspendido
      }
    }
    i++;
  }
  return tipoUsuario;
}

/*------------------------------------------------------------------------*/
/*                               MENU CLIENTE                             */
/*------------------------------------------------------------------------*/

void menuCliente(int usuarioLogueado, solicitudes_t solicitudes[],
                 chofer_t choferes[], int* cantSolicitudes, int* cantCanceladas,
                 char ubicaciones[][TEXTO], char situaciones[][TEXTO],
                 char tiposVehiculos[][TEXTO], reclamo_t reclamos[],
                 int* cantReclamos, perfil_t perfiles[]) {
  int opcion = -1;
  int i;
  int choferEncontrado;
  bool solicitudConfirmada;
  while (opcion != 0) {
    system("cls");
    printf("====================================================\n");
    printf("                  MENU CLIENTE                      \n");
    printf("====================================================\n");
    printf("1. Solicitar auxilio\n");
    printf("2. Consultar estado del viaje\n");
    printf("3. Calificar servicio\n");
    printf("4. Contactar a soporte\n");
    printf("5. Ver historial\n");
    printf("0. Cerrar sesion\n");
    printf("----------------------------------------------------\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
      case 1:
        solicitudConfirmada = registrarSolicitud(
            usuarioLogueado, solicitudes, cantSolicitudes, cantCanceladas,
            ubicaciones, situaciones, tiposVehiculos);
        if (solicitudConfirmada) {
          choferEncontrado =
              motorDeBusqueda(choferes, solicitudes, *cantSolicitudes);
          motorDeAsignacion(choferes, solicitudes, choferEncontrado,
                            *cantSolicitudes - 1);
        }
        system("pause");
        break;

      case 2:
        printf("Seleccionaste: Consultar estado de la solicitud\n");
        system("cls");
        consultaSolicitud(usuarioLogueado, solicitudes, *cantSolicitudes,
                          ubicaciones, tiposVehiculos, situaciones);
        system("pause");
        break;

      case 3:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 4:
        hacerReclamo(usuarioLogueado, reclamos, cantReclamos, perfiles);
        break;

      case 5:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 0:
        system("cls");
        printf("Cerrando sesion...\n");
        Sleep(1500);
        break;

      default:
        printf("Opción incorrecta.\n");
        system("pause");
        break;
    }
  }
}

/* MENU CLIENTE: OPCION 1 > Solicitar auxilio */
bool registrarSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],
                        int* cantSolicitudes, int* cantCanceladas,
                        char ubicaciones[][TEXTO], char situaciones[][TEXTO],
                        char tiposVehiculos[][TEXTO]) {
  int opcionUbicacion, opcionVehiculos, opcionSituacion, confirmar, i,
      prioridad;
  bool confirmada = false;
  bool puedeRegistrar = true;
  system("cls");

  /* Buscar si el cliente ya tiene una solicitud activa */
  for (i = 0; i < *cantSolicitudes; i++) {
    if (solicitudes[i].idCliente == usuarioLogueado &&
        (solicitudes[i].estado == SOLICITUD_PENDIENTE ||
         solicitudes[i].estado == CHOFER_ASIGNADO ||
         solicitudes[i].estado == VIAJE_INICIADO)) {
      printf("Usted ya posee una solicitud activa.\n");
      puedeRegistrar = false;
      break;
    }
  }

  /* Verificar que exista espacio en el vector(que no haya superado la cantidad
   * de solicitudes) */
  if (puedeRegistrar == true && *cantSolicitudes >= CANT_SOLICITUDES) {
    printf("No es posible registrar más solicitudes.\n");
    puedeRegistrar = false;
  }
  /*En caso de poder registrarse comienza con el cuestionario*/
  if (puedeRegistrar == true) {
    do {
      mostrarSolicitudAuxilio();
      printf("Seleccione la ubicación\n");
      printf("1. Merlo\n");
      printf("2. Ituzaingo\n");
      printf("3. Moron\n");
      printf("Opcion: ");
      scanf("%d", &opcionUbicacion);
      if (opcionUbicacion < 1 || opcionUbicacion > 3) {
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
      }
    } while (opcionUbicacion < 1 || opcionUbicacion > 3);
    solicitudes[*cantSolicitudes].ubicacion = opcionUbicacion;
    system("cls");

    do {
      mostrarSolicitudAuxilio();
      printf("Seleccione el tipo de vehículo\n");
      printf("1. Moto\n");
      printf("2. Auto\n");
      printf("3. Camioneta\n");
      printf("Opcion: ");
      scanf("%d", &opcionVehiculos);
      if (opcionVehiculos < 1 || opcionVehiculos > 3) {
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
      }
    } while (opcionVehiculos < 1 || opcionVehiculos > 3);
    solicitudes[*cantSolicitudes].tipoVehiculo = opcionVehiculos;
    system("cls");

    do {
      mostrarSolicitudAuxilio();
      printf("Seleccione la situación\n");
      printf("1. Pinchadura\n");
      printf("2. Falla mecánica\n");
      printf("3. Accidente/choque\n");
      printf("Opcion: ");
      scanf("%d", &opcionSituacion);
      if (opcionSituacion < 1 || opcionSituacion > 3) {
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
      }
    } while (opcionSituacion < 1 || opcionSituacion > 3);
    solicitudes[*cantSolicitudes].situacion = opcionSituacion;

    system("cls");
    do {
      mostrarSolicitudAuxilio();
      printf("Ingrese su patente (ej: abc123):\n");
      printf("Patente: ");
      scanf("%29s", solicitudes[*cantSolicitudes].patente);
      system("cls");

      if (!validarPatente(solicitudes[*cantSolicitudes].patente)) {
        mostrarSolicitudAuxilio();
        printf(
            "Patente incorrecta. Debe contener 3 letras seguidas de 3 numeros "
            "(ej: abc123).\n");
        Sleep(3500);
        system("cls");
      }
    } while (!validarPatente(solicitudes[*cantSolicitudes].patente));

    prioridad = calcularPrioridad();
    solicitudes[*cantSolicitudes].prioridad = prioridad;

    printf("--------------------------------------\n");
    printf("     Los datos seleccionados son:     \n");
    printf("--------------------------------------\n");
    printf("Ubicación: %s\n", ubicaciones[opcionUbicacion - 1]);
    printf("Tipo de vehículo: %s\n", tiposVehiculos[opcionVehiculos - 1]);
    printf("Situación: %s\n", situaciones[opcionSituacion - 1]);
    printf("Patente: %s\n", solicitudes[*cantSolicitudes].patente);
    printf("Nivel de prioridad: ");
    mostrarPrioridad(prioridad);
    printf("..............................................................\n");
    printf("Para avanzar con su solicitud, seleccione la opción deseada.  \n");
    printf("..............................................................\n");
    printf("1. Confirmar solicitud\n");
    printf("2. Cancelar solicitud\n");
    printf("Opcion: ");
    scanf("%d", &confirmar);
    system("cls");

    while (confirmar != 1 && confirmar != 2) {
      printf("Opción incorrecta. Por favor, intente nuevamente.\n");
      printf("Opcion: ");
      scanf("%d", &confirmar);
    }

    if (confirmar == 1) {
      solicitudes[*cantSolicitudes].idCliente = usuarioLogueado;
      solicitudes[*cantSolicitudes].estado = SOLICITUD_PENDIENTE;
      solicitudes[*cantSolicitudes].choferAsignado = -1;
      (*cantSolicitudes)++;
      confirmada = true;
    } else {
      (*cantCanceladas)++;
      printf("La solicitud fue cancelada.\n");
      confirmada = false;
    }
  }
  return confirmada;
}

/* Motor de búsqueda */
int motorDeBusqueda(chofer_t choferes[], solicitudes_t solicitudes[],
                    int cantSolicitudes) {
  int buscaChofer = -1;
  int i;
  for (i = 0; i < CANT_CHOFERES; i++) {
    if (buscaChofer == -1) {
      if (choferes[i].estado == DISPONIBLE) {
        if (choferes[i].ubicacion ==
            solicitudes[cantSolicitudes - 1].ubicacion) {
          if (choferes[i].tipoGrua ==
              solicitudes[cantSolicitudes - 1].tipoVehiculo) {
            buscaChofer = i;
          }
        }
      }
    }
  }
  return buscaChofer;
}

/*Moto de asignación*/
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
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
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
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
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
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
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
  if (strlen(patente) != 6) {  // strlen = tring length = longitud de la cadena,
                               // cuenta cuantos caracteres hay
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

/* MENU CLIENTES OPCION 2 : CONSULTA DE SOLICITUD*/
void consultaSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],
                       int cantSolicitudes, char ubicacion[][TEXTO],
                       char tipoVehiculo[][TEXTO], char situacion[][TEXTO]) {
  int i;
  bool solicitudEncontrada = false;
  for (i = 0; i < cantSolicitudes; i++) {
    if (solicitudes[i].idCliente == usuarioLogueado) {
      solicitudEncontrada = true;
      printf("-------------------------------------------\n");
      printf("------------Estado de su solicitud---------\n");
      printf("-------------------------------------------\n");
      printf("\n");
      printf("Cliente ID N°: %d \n", solicitudes[i].idCliente);
      printf("Ubicación: %s\n", ubicacion[solicitudes[i].ubicacion - 1]);
      printf("Tipo de vehículo: %s\n",
             tipoVehiculo[solicitudes[i].tipoVehiculo - 1]);
      printf("Situación: %s\n", situacion[solicitudes[i].situacion - 1]);
      printf("Patente: %s\n", solicitudes[i].patente);
      printf("Estado: ");
      switch (solicitudes[i].estado) {
        case SIN_SOLICITUD:
          printf("Sin solicitud\n");
          break;

        case SOLICITUD_PENDIENTE:
          printf("Pendiente de asignación\n");
          break;

        case CHOFER_ASIGNADO:
          printf("Chofer asignado\n");
          break;

        case VIAJE_INICIADO:
          printf("Viaje iniciado\n");
          break;

        case VIAJE_FINALIZADO:
          printf("Viaje finalizado\n");
          break;

        default:
          printf("Estado desconocido\n");
          break;
      }
    }
  }
  if (solicitudEncontrada == false) {
    printf("No realizó ninguna solicitud\n");
  }
  printf("\n");
  printf("-------------------------------------------\n");
}
/*------------------------------------------------------------------------*/
/*                              MENU CHOFER                               */
/*------------------------------------------------------------------------*/

void menuChofer(int usuarioLogueado, chofer_t choferes[],
                char ubicaciones[][TEXTO], char tiposGrua[][TEXTO],
                reclamo_t reclamos[], int* cantReclamos, perfil_t perfiles[]) {
  int opcion = -1;
  while (opcion != 0) {
    system("cls");
    printf("====================================================\n");
    printf("                  MENU CHOFER                       \n");
    printf("====================================================\n");
    printf("1. Estado actual\n");
    printf("2. Servicio asignado\n");
    printf("3. Finalizar viaje\n");
    printf("4. Contactar a soporte\n");
    printf("5. Historial de ganancias\n");
    printf("0. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
      case 1:
        system("cls");
        estadoActual(usuarioLogueado, choferes, ubicaciones, tiposGrua);
        system("pause");
        break;

      case 2:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 3:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 4:
        hacerReclamo(usuarioLogueado, reclamos, cantReclamos, perfiles);
        break;

      case 5:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 0:
        system("cls");
        printf("Cerrando sesion...\n");
        Sleep(1500);
        break;

      default:
        printf("Opcion incorrecta.\n");
        system("pause");
        break;
    }
  }
}
/* MENU CHOFER OPCION 1: ESTADO ACTUAL*/
void estadoActual(int usuarioLogueado, chofer_t choferes[],
                  char ubicacion[][TEXTO], char tiposGrua[][TEXTO]) {
  int opcionEstado, cambiarEstado;
  int i, indiceChofer = -1;
  system("cls");
  printf("-------------------------------------------------------------\n");
  printf("                        Estado actual                        \n");
  printf("-------------------------------------------------------------\n");
  printf("Por defecto, el sistema le asignara el estado: No disponible \n");
  printf(".............................................................\n");

  for (i = 0; i < CANT_CHOFERES; i++) {
    if (usuarioLogueado == choferes[i].idPerfil) {
      indiceChofer = i;
    }
  }

  if (choferes[indiceChofer].estado == DISPONIBLE) {
    printf("Estado: Disponible\n");
  } else if (choferes[indiceChofer].estado == OCUPADO) {
    printf("Estado: Ocupado\n");
  } else {
    printf("Estado: No disponible\n");
  }
  printf("........................................................\n");

  do {
    printf("1. Cambiar de estado. \n");
    printf("2. Volver al menú principal\n");
    scanf("%d", &opcionEstado);
    if (opcionEstado > 2 || opcionEstado < 1) {
      printf("Opción incorrecta. Por favor, intente nuevamente.\n");
      Sleep(3000);
      system("cls");
    }
  } while (opcionEstado > 2 || opcionEstado < 1);

  if (opcionEstado == 1) {
    do {
      system("cls");
      printf("--------------------------------------\n");
      printf("            CAMBIAR ESTADO            \n");
      printf("--------------------------------------\n");
      printf("1. Disponible\n");
      printf("2. No disponible\n");
      printf("3. Volver al menu principal\n");
      printf("Ingrese una opción: ");
      scanf("%d", &cambiarEstado);
      if (cambiarEstado < 1 || cambiarEstado > 3) {
        printf("Opción incorrecta. Por favor, intente nuevamente.\n");
        Sleep(3000);
        system("cls");
      }
    } while (cambiarEstado < 1 || cambiarEstado > 3);

    switch (cambiarEstado) {
      case 1:
        choferes[indiceChofer].estado = DISPONIBLE;
        printf(
            "El estado ha sido cambiado. Su estado actual es: Disponible.\n");
        break;
      case 2:
        if (choferes[indiceChofer].estado == NO_DISPONIBLE) {
          printf("Ya se encuentra en estado No Disponible.\n");
        } else {
          choferes[indiceChofer].estado = NO_DISPONIBLE;
          printf(
              "El estado ha sido cambiado. Su estado actual es: No "
              "disponible.\n");
        }
        break;
      case 3:
        printf("Volviendo al menu anterior...\n");
        Sleep(1500);
        break;
    }
  }
}

/* MENU CHOFER Y CLIENTE - OPCION 4: Contactar a soporte*/

void hacerReclamo(int usuarioLogueado, reclamo_t reclamos[], int* cantReclamos,
                  perfil_t perfiles[]) {
  char tituloReclamo[TEXTO];
  char acusadoReclamo[TEXTO];
  char contenidoMensaje[TEXTO];
  int limiteCaracteres = 10;  // limite de prueba, cambiar al que se prefiera

  // Validación si supera la cantidad de reclamos declarados
  if (*cantReclamos >= CANT_RECLAMOS) {
    system("cls");
    printf("El sistema de reclamos ha alcanzado su límite máximo.\n");
    system("pause");
    return;
  }

  // 1. Ingresar Título del reclamo
  do {
    system("cls");
    printf("===============================================================\n");
    printf("                         Contactar a Soporte                   \n");
    printf("===============================================================\n");
    printf("Por favor, complete los campos - Presione ENTER para continuar \n");
    printf("---------------------------------------------------------------\n");

    printf("Ingrese el título del reclamo: ");
    // %[^\n]: Lee y guarda todo lo que el usuario escriba, excepto^ cuando
    // detecte que presionó un Enter \n
    scanf(" %[^\n]", tituloReclamo);

    if (strlen(tituloReclamo) == 0) {
      printf(
          "\nLa opción no puede estar incompleta. Por favor, intente "
          "nuevamente.\n");
      Sleep(3000);
    }
  } while (strlen(tituloReclamo) ==
           0);  // si da 0, es porque está vacío el texto

  // 2. Ingresar el nombre del usuario acusado
  do {
    system("cls");
    printf("===============================================================\n");
    printf("                         Contactar a Soporte                   \n");
    printf("===============================================================\n");
    printf("Por favor, complete los campos - Presione ENTER para continuar \n");
    printf("---------------------------------------------------------------\n");
    printf("Título del reclamo: %s\n", tituloReclamo);

    printf("Ingrese el nombre completo del usuario acusado: ");
    scanf(" %[^\n]", acusadoReclamo);

    if (strlen(acusadoReclamo) == 0) {
      printf(
          "\nLa opción no puede estar incompleta. Por favor, intente "
          "nuevamente.\n");
      Sleep(3000);
    }
  } while (strlen(acusadoReclamo) == 0);

  // 3. Ingresar y validar el contenido/mensaje
  do {
    system("cls");
    printf("===============================================================\n");
    printf("                         Contactar a Soporte                   \n");
    printf("===============================================================\n");
    printf("Detalle su reclamo o consulta. (Máximo %d caracteres)\n",
           limiteCaracteres);
    printf("---------------------------------------------------------------\n");
    printf("Título del reclamo: %s\n", tituloReclamo);
    printf("Usuario acusado: %s\n", acusadoReclamo);

    printf("Contenido del mensaje: ");
    scanf(" %[^\n]", contenidoMensaje);

    // validación de texto, no puede superar los carac. establecidos.
    int limTexto = strlen(contenidoMensaje);

    if (limTexto >= 5 &&
        limTexto <= limiteCaracteres) {  // cambiar 5 por el que querramos
      break;
    } else {
      printf("\n");
      if (limTexto < 5) {
        printf(
            "El mensaje es muy breve. Por favor, describa su problema con "
            "mayor detalle\n");
      } else {
        printf("Superó el límite de %d caracteres (escribió %d).\n",
               limiteCaracteres, limTexto);
      }
      Sleep(3000);
    }
  } while (1);

  // Guardar información en la estructura de reclamos
  if (perfiles[usuarioLogueado].tipoPerfil == PERFIL_CLIENTE) {
    strcpy(reclamos[*cantReclamos].tipoUsuario, "Cliente");
  } else {
    strcpy(reclamos[*cantReclamos].tipoUsuario, "Chofer");
  }

  strcpy(reclamos[*cantReclamos].titulo, tituloReclamo);
  strcpy(reclamos[*cantReclamos].acusado, acusadoReclamo);
  strcpy(reclamos[*cantReclamos].mensaje, contenidoMensaje);
  strcpy(reclamos[*cantReclamos].estado,
         "Pendiente");  // El estado inicia en pendiente, para el filtro de
                        // usuario Soporte
  strcpy(reclamos[*cantReclamos].respuesta, "");
  (*cantReclamos)++;  // para que el prox reclamo se guarde en una nueva
                      // posición

  // Mensaje de confirmación
  system("cls");
  printf("---------------------------------------------------------------\n");
  printf(
      "¡Mensaje enviado con éxito! Nuestra área de Soporte se contactará con "
      "usted.\n");
  printf("---------------------------------------------------------------\n");
  system("pause");
  system("cls");
}

/*------------------------------------------------------------------------*/
/*                         MENU ADMINISTRADOR                             */
/*------------------------------------------------------------------------*/

void menuAdmin() {
  int opcion = -1;

  while (opcion != 0) {
    system("cls");

    printf("====================================================\n");
    printf("                   MENU ADMINISTRADOR               \n");
    printf("====================================================\n");
    printf("1. Gestionar usuarios\n");
    printf("2. Gestionar localidades\n");
    printf("3. Consultar estadisticas\n");
    printf("0. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
      case 1:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 2:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 3:
        system("cls");
        printf(
            "Opción temporalmente fuera de servicio. Disculpe las "
            "molestias.\n");
        printf("\n");
        system("pause");
        break;

      case 0:
        system("cls");
        printf("Cerrando sesion...\n");
        Sleep(1500);
        break;

      default:
        printf("Opcion incorrecta.\n");
        system("pause");
        break;
    }
  }
}

/*------------------------------------------------------------------------*/
/*                               MENU SOPORTE                             */
/*------------------------------------------------------------------------*/
void menuSoporte(int usuarioLogueado, reclamo_t reclamos[], int* cantReclamos,
                 perfil_t perfiles[]) {
  int opcion = -1;

  while (opcion != 0) {
    system("cls");

    printf("====================================================\n");
    printf("                    MENU SOPORTE                    \n");
    printf("====================================================\n");
    printf("1. Ver reclamos pendientes\n");
    printf("2. Gestionar usuarios\n");
    printf("3. Ver historial de reclamos\n");
    printf("0. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
      case 1:
        system("cls");
        verReclamosPendientes(reclamos, *cantReclamos);
        break;

      case 2:
        system("cls");
        gestionarEstadoUsuarios(perfiles);
        break;
      case 3:
        system("cls");
        verHistorialReclamos(reclamos, *cantReclamos);
        break;

      case 0:
        system("cls");
        printf("Cerrando sesion...\n");
        Sleep(1500);
        break;

      default:
        printf("Opcion incorrecta.\n");
        system("pause");
        break;
    }
  }
}
/*MENU SOPORTE - Opción 1: Ver reclamos pendientes*/
void verReclamosPendientes(reclamo_t reclamos[], int cantReclamos) {
  int i;
  bool hayPendientes = false;
  int opcionMenu = 0, numReclamo;
  char comentarioSoporte[TEXTO];

  while (opcionMenu != 2) {
    system("cls");
    printf("------------------------------------------------------\n");
    printf("               Reclamos Pendientes                    \n");
    printf("------------------------------------------------------\n\n");

    hayPendientes = false;

    if (cantReclamos > 0) {
      for (i = 0; i < cantReclamos; i++) {
        if (strcmp(reclamos[i].estado, "Pendiente") == 0) {
          hayPendientes = true;
          printf("Reclamo N°: %d\n", i + 1);
          printf("Enviado por: %s\n", reclamos[i].tipoUsuario);
          printf("Estado: %s\n", reclamos[i].estado);
          printf("Título: %s\n", reclamos[i].titulo);
          printf("Usuario acusado: %s\n", reclamos[i].acusado);
          printf("Mensaje: %s\n", reclamos[i].mensaje);
          printf("---------------------------------------------------------\n");
        }
      }
    }

    if (!hayPendientes) {
      printf("No hay incidencias pendientes en este momento.\n\n");
      system("pause");
      break;
    } else {
      printf("\nSeleccione la opcion deseada:\n");
      printf("1. Gestionar reclamos\n");
      printf("2. Volver al menu principal\n");
      printf("Ingrese una opcion: ");
      scanf("%d", &opcionMenu);

      if (opcionMenu == 1) {
        printf("\nIngrese el número de reclamo a gestionar: ");
        scanf("%d", &numReclamo);

        if (numReclamo >= 1 && numReclamo <= cantReclamos &&
            strcmp(reclamos[numReclamo - 1].estado, "Pendiente") == 0) {
          system("cls");
          printf(
              "----------------------------------------------------------\n");
          printf("       GESTIÓN DE RESOLUCIÓN (Reclamo N° %d)              \n",
                 numReclamo);
          printf(
              "----------------------------------------------------------\n");
          printf("Ingrese el comentario o resolución para este reclamo: ");
          scanf(" %[^\n]", comentarioSoporte);

          if (strlen(comentarioSoporte) > 0) {
            strcpy(reclamos[numReclamo - 1].respuesta, comentarioSoporte);
            strcpy(reclamos[numReclamo - 1].estado, "Validado");
            printf("\n¡Reclamo procesado y validado con éxito!\n");
            Sleep(2000);
          } else {
            printf("\nEl comentario no puede estar vacío.\n");
            Sleep(2000);
          }
        } else {
          printf("\nNúmero de reclamo inválido o inexistente.\n");
          Sleep(2000);
        }
      }
    }
  }
}

/*MENU SOPORTE - Opción 2: Gestionar usuarios*/
void gestionarEstadoUsuarios(perfil_t perfiles[]) {
  int opcionTipoUsuario = 0, numUsuario, opcionSubMenu;

  while (opcionTipoUsuario !=
         3) {  // mantiene al usuario en el submenu hasta que decida salir
    system("cls");
    printf("==========================================\n");
    printf("            Gestion de Usuarios           \n");
    printf("==========================================\n");
    printf("1) Usuarios - Choferes\n");
    printf("2) Usuarios - Clientes\n");
    printf("3) Volver al menu principal\n");
    printf("\nSeleccione el tipo de usuario a gestionar: ");
    scanf("%d", &opcionTipoUsuario);

    if (opcionTipoUsuario == 1 || opcionTipoUsuario == 2) {
      opcionSubMenu = 0;

      // Mantenemos este bucle para ver la lista y las opciones a la vez
      while (opcionSubMenu != 2) {
        system("cls");
        printf("--------------------------------------------------------\n");
        printf("                    Listado de Usuarios                 \n");
        printf("--------------------------------------------------------\n");
        // recorre los perfiles del sistema
        int i;
        for (i = 0; i < CANT_PERFILES; i++) {
          // valida si la opción elegida corresponde a chofer o cliente
          if ((opcionTipoUsuario == 1 &&
               perfiles[i].tipoPerfil == PERFIL_CHOFER) ||
              (opcionTipoUsuario == 2 &&
               perfiles[i].tipoPerfil == PERFIL_CLIENTE)) {
            printf("Perfil Nro. %d | Nombre: %s %s | Estado: %s\n", i,
                   perfiles[i].nombre, perfiles[i].apellido,
                   perfiles[i].estado);
          }
        }

        printf("--------------------------------------------------------\n");
        printf("1. Cambiar estado del usuario (Activar - Suspender)\n");
        printf("2. Volver atras\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcionSubMenu);

        if (opcionSubMenu == 1) {
          printf("--------------------------------------------------------\n");
          printf("\nIngrese el N° de usuario a modificar: ");
          scanf("%d", &numUsuario);

          if (numUsuario >= 0 && numUsuario < CANT_PERFILES) {
            // Validamos que el perfil pertenezca al perfil actual
            int pertenecePerfil =
                (opcionTipoUsuario == 1 &&
                 perfiles[numUsuario].tipoPerfil == PERFIL_CHOFER) ||
                (opcionTipoUsuario == 2 &&
                 perfiles[numUsuario].tipoPerfil == PERFIL_CLIENTE);

            if (pertenecePerfil) {
              // Compara dos textos para ver si son iguales, si es = da 0
              if (strcmp(perfiles[numUsuario].estado, "Activo") == 0) {
                // Copia el texto dentro de otro
                strcpy(perfiles[numUsuario].estado, "Suspendido");
                printf("\n¡Usuario suspendido con exito!\n");
              } else {
                strcpy(perfiles[numUsuario].estado, "Activo");
                printf("\n¡Usuario activado con exito!\n");
              }
            } else {
              printf("\nNumero de usuario inválido. \n");
            }
            Sleep(2000);
          } else {
            printf("\nOpcion invalida. Intente nuevamente.\n");
            Sleep(2000);
          }
        }
      }
    }
  }
}

/*MENU SOPORTE - Opción 3: Ver historial de reclamos*/
void verHistorialReclamos(reclamo_t reclamos[], int cantReclamos) {
  int i;
  bool hayHistorial = false;

  system("cls");
  printf("------------------------------------------------------\n");
  printf("                Historial de Reclamos                 \n");
  printf("------------------------------------------------------\n\n");

  if (cantReclamos > 0) {
    for (i = 0; i < cantReclamos; i++) {
      hayHistorial = true;
      printf("Reclamo N°: %d\n", i + 1);
      printf("Título: %s\n", reclamos[i].titulo);
      printf("Enviado por: %s\n", reclamos[i].tipoUsuario);
      printf("Usuario acusado: %s\n", reclamos[i].acusado);
      printf("Mensaje original: %s\n", reclamos[i].mensaje);
      printf("Estado actual: %s\n", reclamos[i].estado);

      if (strcmp(reclamos[i].estado, "Validado") == 0) {
        printf("Resolución - Comentario: %s\n", reclamos[i].respuesta);
      } else {
        printf("Resolución - Comentario: >> Pendiente de resolución <<\n");
      }
      printf("---------------------------------------------------------\n");
    }
  }

  if (!hayHistorial) {
    printf("No hay registros en el historial de reclamos.\n");
  }

  printf("\n");
  system("pause");
}
