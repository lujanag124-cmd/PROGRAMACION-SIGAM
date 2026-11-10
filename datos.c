#include <stdio.h>
#include "sigam.h"

void guardarPerfiles(perfil_t perfiles[]) {
    FILE *archivo;

    archivo = fopen("perfiles.txt", "w");

    if (archivo == NULL) {
        printf("Error al guardar los perfiles.\n");
    } else {
        for (int i = 0; i < CANT_PERFILES; i++) {
            fprintf(archivo, "%d %s %s %s %s %s\n",
                    perfiles[i].tipoPerfil,
                    perfiles[i].nombre,
                    perfiles[i].apellido,
                    perfiles[i].usuario,
                    perfiles[i].clave,
                    perfiles[i].estado);
        }
        fclose(archivo);
    }
}

int cargarPerfiles(perfil_t perfiles[]) {
    FILE *archivo;
    int i = 0;
    archivo = fopen("perfiles.txt", "r");

    if (archivo == NULL) {
        return 0;
    } else {
        while (i < CANT_PERFILES && (fscanf(archivo, "%d %s %s %s %s %s",
              &perfiles[i].tipoPerfil,
              perfiles[i].nombre,
              perfiles[i].apellido,
              perfiles[i].usuario,
              perfiles[i].clave,
              perfiles[i].estado) == 6)) {
    i++;
}
        fclose(archivo);
    }
    return i;
}

void guardarChoferes(chofer_t choferes[]) {
    FILE *archivo;
    archivo = fopen("choferes.txt", "w");

    if (archivo == NULL) {
        printf("Error al guardar los choferes.\n");
    } else {
        for (int i = 0; i < CANT_CHOFERES; i++) {
            fprintf(archivo, "%d %d %d %d %d\n",
                    choferes[i].idPerfil,
                    choferes[i].estado,
                    choferes[i].ubicacion,
                    choferes[i].tipoGrua,
                    choferes[i].solicitudAsignada);
        }

        fclose(archivo);
    }
}

int cargarChoferes(chofer_t choferes[]) {
    FILE *archivo;
    int i = 0;

    archivo = fopen("choferes.txt", "r");

    if (archivo == NULL) {
        return 0;
    } else {
        while (i < CANT_CHOFERES &&
               fscanf(archivo, "%d %d %d %d %d",
                      &choferes[i].idPerfil,
                      &choferes[i].estado,
                      &choferes[i].ubicacion,
                      &choferes[i].tipoGrua,
                      &choferes[i].solicitudAsignada) == 5) {
            i++;
        }

        fclose(archivo);
    }

    return i;
}

void guardarSolicitudes(solicitudes_t solicitudes[], int cantSolicitudes) {
    FILE *archivo;
    archivo = fopen("solicitudes.txt", "w");

    if (archivo == NULL) {
        printf("Error al guardar las solicitudes.\n");
    } else {
        for (int i = 0; i < cantSolicitudes; i++) {
            fprintf(archivo, "%d %d %d %d %s %d %d %d\n",
                    solicitudes[i].idCliente,
                    solicitudes[i].ubicacion,
                    solicitudes[i].tipoVehiculo,
                    solicitudes[i].situacion,
                    solicitudes[i].patente,
                    solicitudes[i].estado,
                    solicitudes[i].choferAsignado,
                    solicitudes[i].prioridad);
        }
        fclose(archivo);
    }
}

int cargarSolicitudes(solicitudes_t solicitudes[]) {
    FILE *archivo;
    int i = 0;

    archivo = fopen("solicitudes.txt", "r");

    if (archivo == NULL) {
        return 0;
    } else {
        while (i < CANT_SOLICITUDES &&
               fscanf(archivo, "%d %d %d %d %s %d %d %d",
                      &solicitudes[i].idCliente,
                      &solicitudes[i].ubicacion,
                      &solicitudes[i].tipoVehiculo,
                      &solicitudes[i].situacion,
                      solicitudes[i].patente,
                      &solicitudes[i].estado,
                      &solicitudes[i].choferAsignado,
                      &solicitudes[i].prioridad) == 8) {
            i++;
        }
        fclose(archivo);
    }
    return i;
}