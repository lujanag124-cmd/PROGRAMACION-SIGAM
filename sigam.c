#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien
#include <windows.h> //lo usamos para esperar x cantidad de tiempo

#define CANT_CHOFERES 3
#define CANT_CLIENTES 3
#define CANT_ADMIN 1
#define CANT_SOPORTE 1
#define CANT_SOLICITUDES 10
#define TEXTO 30
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
//Definicion de las estructuras de los perfiles
typedef struct{
    int idChofer;
    char usuario[TEXTO];
    char clave[TEXTO];
}chofer_t;

typedef struct{
    int idCliente;
    char usuario[TEXTO];
    char clave[TEXTO];
}cliente_t;

typedef struct{
    char usuario[TEXTO];
    char clave[TEXTO];
}admin_t;

typedef struct{
    char usuario[TEXTO];
    char clave[TEXTO];
}soporte_t;
//----------------------------Estructura de la solicitud
typedef struct{
    int idCliente;
    char ubicacion[TEXTO];
    char patente[TEXTO];
    int tipoVehiculo;
    int estado;
    int choferAsignado;
}solicitudes_t;

//Declaracion de los prototipos
void mostrarBienvenida();
void inicioSesion(char usuario[], char password[]);
int validaLogin (char usuario[], char password[],chofer_t chofer[],cliente_t cliente[],admin_t admin[],soporte_t soporte[],int *choferLogueado, int *clienteLogueado);
void menuCliente(int clienteLogueado, solicitudes_t solicitudes[]);
void menuChofer();
void menuAdmin();
void menuSoporte();
//Inicio del algoritmo
int main() { 
    
    //Declaracion de los vectores de las estructuras
    chofer_t chofer[CANT_CHOFERES];
    cliente_t cliente[CANT_CLIENTES];
    admin_t admin[CANT_ADMIN];
    soporte_t soporte[CANT_SOPORTE];
    solicitudes_t solicitudes[CANT_SOLICITUDES];

    //Credenciales hardcodeadas
    //strcpy() viene de <string.h> copia una cadena dentro de un campo char[] de la estructura
    chofer[0].idChofer = 0;
    strcpy(chofer[0].usuario, "chofer1");
    strcpy(chofer[0].clave,"chofer123");

    chofer[1].idChofer = 1;
    strcpy(chofer[1].usuario, "chofer2");
    strcpy(chofer[1].clave,"chofer123");

    chofer[2].idChofer = 2;
    strcpy(chofer[2].usuario, "chofer3");
    strcpy(chofer[2].clave,"chofer123");
    //--------------------------------------------
    cliente[0].idCliente = 0;
    strcpy(cliente[0].usuario, "cliente1");
    strcpy(cliente[0].clave,"cliente123");

    cliente[1].idCliente = 1;
    strcpy(cliente[1].usuario, "cliente2");
    strcpy(cliente[1].clave,"cliente123");

    cliente[2].idCliente = 2;
    strcpy(cliente[2].usuario, "cliente3");
    strcpy(cliente[2].clave,"cliente123");
    //--------------------------------------------
    strcpy(admin[0].usuario, "admin1");
    strcpy(admin[0].clave,"admin123");
    //--------------------------------------------
    strcpy(soporte[0].usuario, "soporte1");
    strcpy(soporte[0].clave,"soporte123");
    

    char usuario[50], password[50];
    int tipoUsuario, valorUsuario;
    int choferLogueado = -1, clienteLogueado = -1;
    int i;
    //Realiza el conteo de clientes y asu vez los asigna sin solicitud y sin chofer
    for (i=0; i<CANT_CLIENTES; i++){
        solicitudes[i].idCliente = i ;
        solicitudes[i].estado = SIN_SOLICITUD;
        solicitudes[i].choferAsignado = -1;

    }
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
            valorUsuario=validaLogin(usuario,password,chofer,cliente,admin,soporte,&choferLogueado,&clienteLogueado);
            
            if (valorUsuario==LOGIN_INCORRECTO){
                printf("Usuario o clave incorrecta. Por favor, intente nuevamente.\n");
                Sleep(2500);
                system("cls");
            }
        }
        switch (valorUsuario)
        {
            case PERFIL_CHOFER:
                printf("Inicio de sesion correcto.\n");
                printf("Perfil: Chofer ID : %d\n",chofer[choferLogueado].idChofer);
                Sleep(2500);
                menuChofer();
                break;
            case PERFIL_CLIENTE:
                printf("Inicio de sesion correcto.\n");
                printf("Perfil: Cliente. ID : %d\n", cliente[clienteLogueado].idCliente);
                Sleep(2500);
                menuCliente(clienteLogueado,solicitudes);   
                break;
            case PERFIL_ADMIN:
                printf("Inicio de sesion correcto.\n");
                printf("Perfil: Admin\n");
                Sleep(1500);
                menuAdmin();
                break;
            case PERFIL_SOPORTE:
                printf("Inicio de sesion correcto.\n");
                printf("Perfil: Soporte\n");
                menuSoporte();
                break;
            default:
                printf("Error inesperado al identificar el perfil.\n");
                break;
        }
        printf("Desea volver a iniciar sesion?\n");
        printf("1- SI\n");
        printf("0- NO, cerrar SIGAM\n");
        scanf("%d", &continuar); 
       
        valorUsuario=LOGIN_INCORRECTO;
        choferLogueado = -1;
        clienteLogueado = -1;
    }
    return 0;
}

void mostrarBienvenida() {
    printf("====================================================\n");
    printf("                BIENVENIDO/A A SIGAM                \n");
    printf("====================================================\n");
    printf("Sistema de Gestion y Asignacion de Auxilio Mecanico \n");
	printf("....................................................\n");
}


// Login >> usuario ingresa sus credenciales
void inicioSesion(char usuario[], char password[]) {
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);
    printf("Ingrese su clave: ");
    scanf("%s", password);
}
//Aclaraciones:La funcion devuelve una constante que identifica el perfil.
// PERFIL_CLIENTE, PERFIL_CHOFER, PERFIL_ADMIN o PERFIL_SOPORTE.
//strcmp() compara dos cadenas y devuelve 0 cuando son iguales.
// usuario y password se comparan sin [] porque eso devolveria una sola letra justamente por lo dicho arriba 
//Incorpore clienteLogueado asi tambien sabemos que cliente es
int validaLogin(char usuario[], char password[],chofer_t chofer[],cliente_t cliente[],admin_t admin[],soporte_t soporte[],int *choferLogueado, int *clienteLogueado){
    int i=0;
    int tipoUsuario=LOGIN_INCORRECTO;
    
    while (i<CANT_CHOFERES && tipoUsuario == LOGIN_INCORRECTO){
        if(strcmp(usuario, chofer[i].usuario)==0 && strcmp(password,chofer[i].clave)==0){
            tipoUsuario= PERFIL_CHOFER;
            *choferLogueado=i;
        }
        i++;
    }
    i=0;
    while (i<CANT_CLIENTES && tipoUsuario== LOGIN_INCORRECTO){
        if(strcmp(usuario, cliente[i].usuario)==0 && strcmp(password,cliente[i].clave)==0){
            tipoUsuario= PERFIL_CLIENTE;
            *clienteLogueado=i;
        }
        i++;
    }
    i=0;
    while (i<CANT_ADMIN && tipoUsuario== LOGIN_INCORRECTO){
        if(strcmp(usuario, admin[i].usuario)==0 && strcmp(password,admin[i].clave)==0){
            tipoUsuario= PERFIL_ADMIN;
        }
        i++;
    }
    i=0;
    while (i<CANT_SOPORTE && tipoUsuario== LOGIN_INCORRECTO){
        if(strcmp(usuario, soporte[i].usuario)==0 && strcmp(password,soporte[i].clave)==0){
            tipoUsuario= PERFIL_SOPORTE;
        }
        i++;
    }
    return tipoUsuario;
} 
void menuCliente(int clienteLogueado, solicitudes_t solicitudes[]) {
    int opcion=-1;
    while (opcion != 0){
        system("cls");
        printf("====================================================\n");
        printf("                  MENU CLIENTE                      \n");
        printf("====================================================\n");
        printf("1. Solicitar auxilio mecanico\n");
        printf("2. Consultar estado de la solicitud\n");
        printf("3. Inciar viaje\n");
        printf("4. Finalizar viaje\n");
        printf("0. Cerrar sesion\n");
        printf("----------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion){
            case 1:
                printf("Seleccionaste: Solicitar auxilio mecanico\n");
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Consultar estado de la solicitud\n");
                system("pause");
                break;

            case 3:
                printf("Seleccinaste: Inciar viaje\n");
                system("pause");
                break;
            
            case 4:
                printf("Seleccionaste: Finalizar viaje\n");
                system("pause");
                break;
                
            case 0:
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
void menuChofer() {
    int opcion=-1;
    while (opcion != 0){
        system("cls");
        printf("====================================================\n");
        printf("                  MENU CHOFER                       \n");
        printf("====================================================\n");
        printf("1. Ver solicitud asignada\n");
        printf("2. Aceptar solicitud\n");
        printf("3. Iniciar viaje\n");
        printf("4. Finalizar viaje\n");
        printf("0. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion){
            case 1:
                printf("Seleccionaste: Ver solicitud asignada\n");
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Aceptar solicitud\n");
                system("pause");
                break;

            case 3:
                printf("Seleccionaste: Iniciar viaje\n");
                system("pause");
                break;

            case 4:
                printf("Seleccionaste: Finalizar viaje\n");
                system("pause");
                break;

            case 0:
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
void menuAdmin() {
    int opcion = -1;

    while (opcion != 0) {
        system("cls");

        printf("========== MENU ADMINISTRADOR ==========\n");
        printf("1. Gestionar clientes\n");
        printf("2. Gestionar choferes\n");
        printf("3. Consultar solicitudes\n");
        printf("0. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Seleccionaste: Gestionar clientes\n");
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Gestionar choferes\n");
                system("pause");
                break;

            case 3:
                printf("Seleccionaste: Consultar solicitudes\n");
                system("pause");
                break;

            case 0:
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
void menuSoporte() {
    int opcion = -1;

    while (opcion != 0) {
        system("cls");

        printf("============== MENU SOPORTE ==============\n");
        printf("1. Consultar incidencias\n");
        printf("2. Registrar incidencia\n");
        printf("0. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Seleccionaste: Consultar incidencias\n");
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Registrar incidencia\n");
                system("pause");
                break;

            case 0:
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