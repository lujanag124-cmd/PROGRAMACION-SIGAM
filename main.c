#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "sigam.h"


//Inicio del algoritmo
int main() { 
    
    //Declaracion de los vectores de las estructuras y credenciales hardcodeadas
    perfil_t perfiles[CANT_PERFILES]= {
        {PERFIL_CHOFER,"Carlos", "Rodriguez", "chofer1", "chofer123"},
        {PERFIL_CHOFER,"Sandra", "Sanchez", "chofer2", "chofer123"},
        {PERFIL_CHOFER,"Pablo", "Escobar", "chofer3", "chofer123"},

        {PERFIL_CLIENTE,"Tomas", "Escobar","cliente1", "cliente123"},
        {PERFIL_CLIENTE,"Omar", "Gomez","cliente2", "cliente123"},
        {PERFIL_CLIENTE,"Agustina", "Gonzalez", "cliente3", "cliente123"},

        {PERFIL_ADMIN,"Julian","Alvarez",  "admin1","admin123"},
        {PERFIL_SOPORTE,"Lionel","Messi",  "soporte1","soporte123"}
    };

    chofer_t choferes[CANT_CHOFERES] = {
        {0, NO_DISPONIBLE, MERLO, GRUA_MOTO, -1},
        {1, NO_DISPONIBLE, ITUZAINGO, GRUA_AUTO, -1},
        {2, NO_DISPONIBLE, MORON, GRUA_CAMIONETA, -1}
    };

    char tiposGrua[CANT_POSIBILIDADES][TEXTO] = {
        "Grua para moto",
        "Grua liviana(auto)",
        "Grua para camioneta"
    };
    solicitudes_t solicitudes[CANT_SOLICITUDES];

    char ubicaciones[CANT_POSIBILIDADES][TEXTO] = {
        "Merlo",
        "Ituzaingo",
        "Moron"
    };

    char tiposVehiculos[CANT_POSIBILIDADES][TEXTO] = {
        "Moto",
        "Auto",
        "Camioneta"
    };

    char situaciones[CANT_POSIBILIDADES][TEXTO] = {
        "Pinchadura",
        "Falla mecanica",
        "Accidente/choque"
    };
    
    char usuario[50], password[50];
    int valorUsuario,cantSolicitudes=0,cantCanceladas=0;
    int usuarioLogueado = -1;
 
    //system("pause"); equivalente a esperar tecla
    //system("cls"); equivalente a limpiar pantalla
    //Sleep(3000); // 3 segundos   este es para esperar X segundos, se escribe en milisegundos
 
    valorUsuario=LOGIN_INCORRECTO;
    int continuar = 1 ;
    while (continuar == 1) //--------Para poder inicializar con otro perfil y/o salir del programa en su defecto.
    {

        while(valorUsuario==LOGIN_INCORRECTO){ 
            system("cls");
            //system("cls"); equivalente a limpiar pantalla
            mostrarBienvenida();

            // Inicio de sesión
            inicioSesion(usuario, password);

            //Validacion para credenciales incorrectas en login
            valorUsuario=validaLogin(usuario,password,perfiles,&usuarioLogueado);
            
            if (valorUsuario==LOGIN_INCORRECTO){
                printf("Usuario o clave incorrecta. Por favor, intente nuevamente.\n");
                Sleep(1500);
                system("cls");
            }
        }
        switch (valorUsuario)
        {
            case PERFIL_CHOFER:
                system("cls");
                printf("Inicio de sesion correcto.\n");
                printf("Bienvenido/a  %s %s\n", perfiles[usuarioLogueado].nombre, perfiles[usuarioLogueado].apellido);
                Sleep(2500);
                menuChofer(usuarioLogueado,choferes,ubicaciones,tiposGrua);
                break;
            case PERFIL_CLIENTE:
                system("cls");
                printf("Inicio de sesion correcto.\n");
                printf("Bienvenido/a  %s %s\n", perfiles[usuarioLogueado].nombre, perfiles[usuarioLogueado].apellido);
                Sleep(2500);
                menuCliente(usuarioLogueado,solicitudes,choferes,&cantSolicitudes,&cantCanceladas,ubicaciones,situaciones,tiposVehiculos);   
                break;
            case PERFIL_ADMIN:
                system("cls");
                printf("Inicio de sesion correcto.\n");
                printf("Bienvenido/a  %s %s\n", perfiles[usuarioLogueado].nombre, perfiles[usuarioLogueado].apellido);
                Sleep(1500);
                menuAdmin();
                break;
            case PERFIL_SOPORTE:
                system("cls");
                printf("Inicio de sesion correcto.\n");
                printf("Bienvenido/a  %s %s\n", perfiles[usuarioLogueado].nombre, perfiles[usuarioLogueado].apellido);
                menuSoporte();
                break;
            default:
                printf("Error inesperado al identificar el perfil.\n");
                break;
        }
        system("cls");
        do{
        printf("Desea volver a iniciar sesion?\n");
        printf("1- SI\n");
        printf("0- NO, quiero cerrar SIGAM\n");
        printf("Opcion: ");
        scanf("%d", &continuar); 
        system("cls");
        }while (continuar < 0 || continuar > 1);
        valorUsuario=LOGIN_INCORRECTO;
        usuarioLogueado = -1;
    
    }
    return 0;
}