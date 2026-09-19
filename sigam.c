#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien


void mostrarBienvenida() {
    printf("====================================================\n");
    printf("                BIENVENIDO/A A SIGAM                \n");
    printf("====================================================\n");
    printf("Sistema de Gestion y Asignacion de Auxilio Mecanico \n");
	  printf("....................................................\n");
}

void inicioSesion(char usuario[], char password[]);

int main() { 

    char usuario[50], password[50];

    //Mensaje de bienvenida
     mostrarBienvenida();
    
     // Inicio de sesión
    inicioSesion(usuario, password);
    


  return 0;
}


// Login >> usuario ingresa sus credenciales
void inicioSesion(char usuario[], char password[]) {
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);
    printf("Ingrese su contrasenia: ");
    scanf("%s", password);
}
 