#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "sigam.h"

/*------------------------------------------------------------------------*/
/*                         MENU ADMINISTRADOR                             */
/*------------------------------------------------------------------------*/

void menuAdmin(perfil_t perfiles[], char ubicaciones[][TEXTO],
               int estadosLocalidades[]) {
  int opcion = -1;

  while (opcion != 0) {
    system("cls");

    printf("====================================================\n");
    printf("                   MENU ADMINISTRADOR               \n");
    printf("====================================================\n");
    printf("1. Gestionar usuarios\n");
    printf("2. Gestionar localidades\n");
    printf("0. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
      case 1:
        system("cls");
        gestionarEstadoUsuarios(perfiles);
        break;

      case 2:
        system("cls");
        gestionarLocalidades(ubicaciones, estadosLocalidades);
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

/*MENU SOPORTE / Admin - Opción: Gestionar usuarios*/
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
        int contador = 1;

        for (i = 0; i < CANT_PERFILES; i++) {
          // valida si la opción elegida corresponde a chofer o cliente
          if ((opcionTipoUsuario == 1 &&
               perfiles[i].tipoPerfil == PERFIL_CHOFER) ||
              (opcionTipoUsuario == 2 &&
               perfiles[i].tipoPerfil == PERFIL_CLIENTE)) {
            printf("Perfil Nro. %d | Nombre: %s %s | Estado: %s\n", contador,
                   perfiles[i].nombre, perfiles[i].apellido,
                   perfiles[i].estado);

            contador++;
          }
        }

        printf("--------------------------------------------------------\n");
        printf("1. Cambiar estado del usuario (Activar - Suspender)\n");
        printf("2. Volver atras\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcionSubMenu);

        if (opcionSubMenu == 1) {
          int contador = 0;
          int indiceUsuario = -1;
          int i;

          printf("------------------------------------------\n");
          printf("Ingrese el Nro. de usuario a modificar: ");
          scanf("%d", &numUsuario);

          // Buscamos el usuario seleccionado
          for (i = 0; i < CANT_PERFILES; i++) {
            if ((opcionTipoUsuario == 1 &&
                 perfiles[i].tipoPerfil == PERFIL_CHOFER) ||
                (opcionTipoUsuario == 2 &&
                 perfiles[i].tipoPerfil == PERFIL_CLIENTE)) {
              contador++;

              if (contador == numUsuario) {
                indiceUsuario = i;
                break;
              }
            }
          }

          // Verificamos si encontramos al usuario
          if (indiceUsuario != -1) {
            if (strcmp(perfiles[indiceUsuario].estado, "Activo") == 0) {
              strcpy(perfiles[indiceUsuario].estado, "Suspendido");
              printf("\nUsuario suspendido con exito!\n");

            } else {
              strcpy(perfiles[indiceUsuario].estado, "Activo");
              printf("\nUsuario activado con exito!\n");
            }
            guardarPerfiles(perfiles);
          } else {
            printf("\nNumero de usuario invalido.\n");
          }

          Sleep(2000);
          continue;
        }
      }
    }
  }
}
/* MENU ADMINISTRADOR - OPCION 2: GESTIONAR LOCALIDADES */

void gestionarLocalidades(char ubicaciones[][TEXTO], int estadosLocalidades[]) {
  int i;
  int opcion = 0;
  int numLocalidad;

  while (opcion != 2) {
    system("cls");

    printf("========================================\n");
    printf("        GESTION DE LOCALIDADES          \n");
    printf("========================================\n");

    // Mostrar localidades y sus estados
    for (i = 0; i < CANT_POSIBILIDADES; i++) {
      printf("%d. %s | Estado: ", i + 1, ubicaciones[i]);

      if (estadosLocalidades[i] == 1) {
        printf("Activa\n");
      } else {
        printf("Suspendida\n");
      }
    }

    printf("----------------------------------------\n");
    printf("1. Cambiar estado de una localidad\n");
    printf("2. Volver al menu Administrador\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
      case 1:
        printf("\nIngrese el numero de localidad: ");
        scanf("%d", &numLocalidad);

        if (numLocalidad >= 1 && numLocalidad <= CANT_POSIBILIDADES) {
          i = numLocalidad - 1;

          if (estadosLocalidades[i] == 1) {
            estadosLocalidades[i] = 0;
            printf("\nLocalidad suspendida con exito!\n");
          } else {
            estadosLocalidades[i] = 1;
            printf("\nLocalidad activada con exito!\n");
          }

        } else {
          printf("\nNumero de localidad invalido.\n");
        }

        Sleep(2000);
        break;

      case 2:
        printf("\nVolviendo al menu Administrador...\n");
        Sleep(1500);
        break;

      default:
        printf("\nOpcion incorrecta.\n");
        Sleep(2000);
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
          printf("Reclamo Nro: %d\n", i + 1);
          printf("Enviado por: %s\n", reclamos[i].tipoUsuario);
          printf("Estado: %s\n", reclamos[i].estado);
          printf("Titulo: %s\n", reclamos[i].titulo);
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
        printf("\nIngrese el numero de reclamo a gestionar: ");
        scanf("%d", &numReclamo);

        if (numReclamo >= 1 && numReclamo <= cantReclamos &&
            strcmp(reclamos[numReclamo - 1].estado, "Pendiente") == 0) {
          system("cls");
          printf(
              "----------------------------------------------------------\n");
          printf(
              "       GESTION DE RESOLUCION (Reclamo Nro %d)              \n",
              numReclamo);
          printf(
              "----------------------------------------------------------\n");
          printf("Ingrese el comentario o resolucion para este reclamo: ");
          scanf(" %[^\n]", comentarioSoporte);

          if (strlen(comentarioSoporte) > 0) {
            strcpy(reclamos[numReclamo - 1].respuesta, comentarioSoporte);
            strcpy(reclamos[numReclamo - 1].estado, "Validado");
            printf("\nReclamo procesado y validado con exito.\n");
            Sleep(2000);
          } else {
            printf("\nEl comentario no puede estar vacio.\n");
            Sleep(2000);
          }
        } else {
          printf("\nNumero de reclamo invalido o inexistente.\n");
          Sleep(2000);
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

