#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien
#include <windows.h> //lo usamos para esperar x cantidad de tiempo

#define CANT_PERFILES 8
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
#define VEHICULO_MOTO 1
#define VEHICULO_AUTO 2
#define VEHICULO_CAMIONETA 3
#define CANT_POSIBILIDADES 3
//Definicion de las estructuras de los perfiles
typedef struct{
    char nombre[TEXTO];
    char apellido[TEXTO];
    int idPerfil;
    char usuario[TEXTO];
    char clave[TEXTO];
}perfil_t;

//----------------------------Estructura de la solicitud
typedef struct{
    int idCliente;
    int ubicacion;
    int tipoVehiculo;
    int situacion;
    char patente[TEXTO];
    int estado;
    int choferAsignado;
}solicitudes_t;

//Declaracion de los prototipos
void mostrarBienvenida();
void inicioSesion(char usuario[], char password[]);
int validaLogin (char usuario[], char password[],perfil_t perfiles[],int *usuarioLogueado);
void menuCliente(int usuarioLogueado, solicitudes_t solicitudes[],int *cantSolicitudes, char ubicaciones[][TEXTO], char situaciones[][TEXTO],char tiposVehiculos[][TEXTO]);
void menuChofer();
void menuAdmin();
void menuSoporte();
bool registrarSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],int *cantSolicitudes, char ubicaciones[][TEXTO], char situaciones[][TEXTO],char tiposVehiculos[][TEXTO]);
//Inicio del algoritmo
int main() { 
    
    //Declaracion de los vectores de las estructuras y credenciales hardcodeadas
    perfil_t perfiles[CANT_PERFILES]= {
        {"Carlos", "Rodriguez", PERFIL_CHOFER, "chofer1", "chofer123"},
        {"Sandra", "Sanchez", PERFIL_CHOFER, "chofer2", "chofer123"},
        {"Pablo", "Escobar", PERFIL_CHOFER, "chofer3", "chofer123"},

        {"Tomas", "Escobar", PERFIL_CLIENTE, "cliente1", "cliente123"},
        {"Omar", "Gomez", PERFIL_CLIENTE, "cliente2", "cliente123"},
        {"Agustina", "Gonzalez", PERFIL_CLIENTE, "cliente3", "cliente123"},

        {"Gabriel", "Avalos", PERFIL_ADMIN, "admin1","admin123"},
        {"Lionel", "Messi", PERFIL_SOPORTE, "soporte1","soporte123"}
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
    int valorUsuario,cantSolicitudes=0;
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
                menuChofer();
                break;
            case PERFIL_CLIENTE:
                system("cls");
                printf("Inicio de sesion correcto.\n");
                printf("Bienvenido/a  %s %s\n", perfiles[usuarioLogueado].nombre, perfiles[usuarioLogueado].apellido);
                Sleep(2500);
                menuCliente(usuarioLogueado,solicitudes, &cantSolicitudes,ubicaciones,situaciones,tiposVehiculos);   
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
        printf("Desea volver a iniciar sesion?\n");
        printf("1- SI\n");
        printf("0- NO, quiero cerrar SIGAM\n");
        scanf("%d", &continuar); 
       
        valorUsuario=LOGIN_INCORRECTO;
        usuarioLogueado = -1;
    
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

int validaLogin(char usuario[], char password[],perfil_t perfiles[],int *usuarioLogueado){
    int i=0;
    int tipoUsuario=LOGIN_INCORRECTO;
    
    while (i<CANT_PERFILES && tipoUsuario == LOGIN_INCORRECTO){
        if(strcmp(usuario, perfiles[i].usuario)==0 && strcmp(password,perfiles[i].clave)==0){
            *usuarioLogueado=i;
            tipoUsuario=perfiles[i].idPerfil;
        }
        i++;
    }
    return tipoUsuario;
} 

void menuCliente(int usuarioLogueado, solicitudes_t solicitudes[],int *cantSolicitudes, char ubicaciones[][TEXTO], char situaciones[][TEXTO],char tiposVehiculos[][TEXTO]) {
    int opcion=-1;
    bool solicitudConfirmada;
    while (opcion != 0){
        system("cls");
        printf("====================================================\n");
        printf("                  MENU CLIENTE                      \n");
        printf("====================================================\n");
        printf("1. Solicitar auxilio mecanico\n");
        printf("2. Consultar estado de la solicitud\n");
        printf("3. Contactar a soporte\n");
        printf("4. Calificar servicio\n");
        printf("5. Ver historial\n");
        printf("0. Cerrar sesion\n");
        printf("----------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                solicitudConfirmada=registrarSolicitud(usuarioLogueado,solicitudes,cantSolicitudes,ubicaciones,situaciones,tiposVehiculos);
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Consultar estado de la solicitud\n");
                system("pause");
                break;

            case 3:
                printf("Seleccionaste: contactar a soporte\n");
                system("pause");
                break;
            
            case 4:
                printf("Seleccionaste: calificar servicio\n");
                system("pause");
                break;
            
            case 5:
                printf("Seleccionaste: ver historial\n");
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

 bool registrarSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],int *cantSolicitudes, char ubicaciones[][TEXTO], char situaciones[][TEXTO],char tiposVehiculos[][TEXTO]){
    int opcionUbicacion,opcionVehiculos,opcionSituacion, confirmar;
    bool confirmada;
    system("cls");

    printf("===============================================================\n");
 	printf("                     Solicitar auxilio                         \n");
	printf("===============================================================\n");
	printf(" Por favor, seleccione y complete las opciones de la solicitud.\n");
	printf("---------------------------------------------------------------\n");

    do{
        printf("Seleccione la ubicación\n");
        printf("1. Merlo\n");
        printf("2. Ituzaingo\n");
        printf("3. Moron\n");
        scanf("%d", &opcionUbicacion);
        if(opcionUbicacion <1 || opcionUbicacion >3){
            system("cls");
            printf("Por favor, ingrese una opcion correcta\n");
        }
    }while(opcionUbicacion <1 || opcionUbicacion >3);
    solicitudes[*cantSolicitudes].ubicacion = opcionUbicacion; 

        do{
        printf("Seleccione el tipo de vehiculo\n");
        printf("1. Moto\n");
        printf("2. Auto\n");
        printf("3. Camioneta\n");
        scanf("%d", &opcionVehiculos);
        if(opcionVehiculos <1 || opcionVehiculos >3){
            system("cls");
            printf("Por favor, ingrese una opcion correcta\n");
        }
    }while(opcionVehiculos <1 || opcionVehiculos >3);
    solicitudes[*cantSolicitudes].tipoVehiculo = opcionVehiculos; 

    do{
        printf("Seleccione la situación\n");
        printf("1. Pinchadura\n");
        printf("2. Falla mecanica\n");
        printf("3. Accidente/choque\n");
        scanf("%d", &opcionSituacion);
        if(opcionSituacion <1 || opcionSituacion >3){
            system("cls");
            printf("Por favor, ingrese una opcion correcta\n");
        }
    }while(opcionSituacion <1 || opcionSituacion >3);
    solicitudes[*cantSolicitudes].situacion = opcionSituacion; 

    printf("Ingrese su patente\n");
    scanf("%29s",solicitudes[*cantSolicitudes].patente);

    system("cls");

    printf("--------------------------------------\n");
    printf("     Los datos seleccionados son:     \n");
    printf("--------------------------------------\n");
    printf("Ubicación: %s\n",ubicaciones[opcionUbicacion-1]);
    printf("Tipo de vehiculo: %s\n",tiposVehiculos[opcionVehiculos-1]);
    printf("Situación: %s\n", situaciones[opcionSituacion-1]);
    printf("Patente: %s\n", solicitudes[*cantSolicitudes].patente);
    printf("..............................................................\n");
    printf("Para avanzar con su solicitud, seleccione la opcion deseada.  \n");
    printf("..............................................................\n");
    printf("1. Confirmar solicitud\n");
    printf("2. Cancelar solicitud\n");
    scanf("%d",&confirmar);

    while(confirmar !=1 && confirmar !=2 ){
        printf("Por favor, ingrese una opcion correcta \n");
        scanf("%d",&confirmar);
    }

    if(confirmar == 1){
        confirmada= true;
        solicitudes[*cantSolicitudes].idCliente= usuarioLogueado;
        (*cantSolicitudes) ++;
    }else if (confirmar== 2){
        confirmada= false;
    }
return confirmada;
}
        

void menuChofer() {
    int opcion=-1;
    while (opcion != 0){
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

        switch (opcion){
            case 1:
                printf("Seleccionaste: Estado actual\n");
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Servicio asignado\n");
                system("pause");
                break;

            case 3:
                printf("Seleccionaste: Finalizar viaje\n");
                system("pause");
                break;

            case 4:
                printf("Seleccionaste: Contactar a soporte\n");
                system("pause");
                break;

            case 5:
                printf("Seleccionaste: Historial de ganancias\n");
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
