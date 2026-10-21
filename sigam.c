#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien
#include <windows.h> //lo usamos para esperar x cantidad de tiempo
#include "sigam.h"


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
            tipoUsuario=perfiles[i].tipoPerfil;
        }
        i++;
    }
    return tipoUsuario;
} 

void menuCliente(int usuarioLogueado, solicitudes_t solicitudes[], chofer_t choferes[],int *cantSolicitudes,int *cantCanceladas, char ubicaciones[][TEXTO], char situaciones[][TEXTO],char tiposVehiculos[][TEXTO]) {
    int opcion=-1;
    int i;
    int choferEncontrado;
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
                solicitudConfirmada=registrarSolicitud(usuarioLogueado,solicitudes,cantSolicitudes,cantCanceladas,ubicaciones,situaciones,tiposVehiculos);
                if (solicitudConfirmada){
                    choferEncontrado = motorDeBusqueda(choferes, solicitudes, *cantSolicitudes);
                    motorDeAsignacion(choferes, solicitudes, choferEncontrado, *cantSolicitudes - 1);

                }
                system("pause");
                break;

            case 2:
                printf("Seleccionaste: Consultar estado de la solicitud\n");
                system("cls");
                consultaSolicitud(usuarioLogueado,solicitudes,*cantSolicitudes,ubicaciones,tiposVehiculos,situaciones);
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
/* MENU CLIENTE: OPCION - 1 */
bool registrarSolicitud(int usuarioLogueado, solicitudes_t solicitudes[],int *cantSolicitudes, int *cantCanceladas, char ubicaciones[][TEXTO], char situaciones[][TEXTO],char tiposVehiculos[][TEXTO])
{
    int opcionUbicacion,opcionVehiculos,opcionSituacion, confirmar,i,prioridad;
    bool confirmada = false;
    bool puedeRegistrar = true;
    system("cls");
      /* Buscar si el cliente ya tiene una solicitud activa */
    for (i = 0; i < *cantSolicitudes; i++)
    {
        if (solicitudes[i].idCliente == usuarioLogueado && (solicitudes[i].estado == SOLICITUD_PENDIENTE || solicitudes[i].estado == CHOFER_ASIGNADO || solicitudes[i].estado == VIAJE_INICIADO))
        {
            printf("Usted ya posee una solicitud activa.\n");
            puedeRegistrar = false;
            break;
        }
    }

    /* Verificar que exista espacio en el vector(que no haya superado la cantidad de solicitudes) */
    if (puedeRegistrar == true && *cantSolicitudes >= CANT_SOLICITUDES)
    {
        printf("No se pueden registrar mas solicitudes.\n");
        puedeRegistrar = false;
    }
    /*En caso de poder registrarse comienza con el cuestionario*/
    if (puedeRegistrar == true)
    { 
        printf("====================================================================\n");
        printf("                          Solicitar auxilio                         \n");
        printf("====================================================================\n");
        printf(" A continuacion, seleccione y complete las opciones de la solicitud.\n");
        printf("--------------------------------------------------------------------\n");
        Sleep(3500);
        system("cls");

        do{
            printf("Seleccione la ubicacion\n");
            printf("1. Merlo\n");
            printf("2. Ituzaingo\n");
            printf("3. Moron\n");
            printf("Opcion: ");
            scanf("%d", &opcionUbicacion);
            if(opcionUbicacion <1 || opcionUbicacion >3)
            {
                system("cls");
                printf("Por favor, ingrese una opcion correcta\n");
            }
        }while(opcionUbicacion <1 || opcionUbicacion >3);
        solicitudes[*cantSolicitudes].ubicacion = opcionUbicacion; 
        system("cls");

        do{
            printf("Seleccione el tipo de vehiculo\n");
            printf("1. Moto\n");
            printf("2. Auto\n");
            printf("3. Camioneta\n");
            printf("Opcion: ");
            scanf("%d", &opcionVehiculos);
            if(opcionVehiculos <1 || opcionVehiculos >3)
            {
                system("cls");
                printf("Por favor, ingrese una opcion correcta\n");
            }
        }while(opcionVehiculos <1 || opcionVehiculos >3);
        solicitudes[*cantSolicitudes].tipoVehiculo = opcionVehiculos; 
        system("cls");

        do{
            printf("Seleccione la situación\n");
            printf("1. Pinchadura\n");
            printf("2. Falla mecanica\n");
            printf("3. Accidente/choque\n");
            printf("Opcion: ");
            scanf("%d", &opcionSituacion);
            if(opcionSituacion <1 || opcionSituacion >3)
            {
                system("cls");
                printf("Por favor, ingrese una opcion correcta\n");
            }
        }while(opcionSituacion <1 || opcionSituacion >3);
        solicitudes[*cantSolicitudes].situacion = opcionSituacion; 

        system("cls");
        printf("Ingrese su patente\n");
        printf("Patente: ");
        scanf("%29s",solicitudes[*cantSolicitudes].patente);
        system("cls");

        prioridad = calcularPrioridad();
        solicitudes[*cantSolicitudes].prioridad = prioridad;

        printf("--------------------------------------\n");
        printf("     Los datos seleccionados son:     \n");
        printf("--------------------------------------\n");
        printf("Ubicación: %s\n",ubicaciones[opcionUbicacion-1]);
        printf("Tipo de vehiculo: %s\n",tiposVehiculos[opcionVehiculos-1]);
        printf("Situación: %s\n", situaciones[opcionSituacion-1]);
        printf("Patente: %s\n", solicitudes[*cantSolicitudes].patente);
        printf("Nivel de prioridad: ");
        mostrarPrioridad(prioridad);
        printf("..............................................................\n");
        printf("Para avanzar con su solicitud, seleccione la opcion deseada.  \n");
        printf("..............................................................\n");
        printf("1. Confirmar solicitud\n");
        printf("2. Cancelar solicitud\n");
        printf("Opcion: ");
        scanf("%d",&confirmar);
        system("cls");

        while(confirmar !=1 && confirmar !=2 )
        {
            printf("Por favor, ingrese una opcion correcta \n");
            printf("Opcion: ");
            scanf("%d",&confirmar);
        }

        if(confirmar == 1)
        {
            solicitudes[*cantSolicitudes].idCliente= usuarioLogueado;solicitudes[*cantSolicitudes].estado=SOLICITUD_PENDIENTE;solicitudes[*cantSolicitudes].choferAsignado=-1;
            (*cantSolicitudes) ++;
            confirmada = true;
        }
        else{
        (*cantCanceladas) ++;
        printf("La solicitud fue cancelada.\n");
        confirmada = false;
        }
    }
    return confirmada;
}

int motorDeBusqueda(chofer_t choferes[], solicitudes_t solicitudes[], int cantSolicitudes){
    int buscaChofer = -1;
    int i;
    for (i = 0; i < CANT_CHOFERES; i++){
        if (buscaChofer == -1){
            if(choferes[i].estado == DISPONIBLE){
                if(choferes[i].ubicacion == solicitudes[cantSolicitudes-1].ubicacion){
                    if (choferes[i].tipoGrua ==solicitudes[cantSolicitudes-1].tipoVehiculo){
                        buscaChofer = i;
                    }
                }
            }
        }
        
    }
    return buscaChofer;
}

void motorDeAsignacion(chofer_t choferes[], solicitudes_t solicitudes[], int choferEncontrado,int indiceSolicitud){
 if (choferEncontrado != -1){
        choferes[choferEncontrado].solicitudAsignada = indiceSolicitud;
        choferes[choferEncontrado].estado = OCUPADO ;
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

int calcularPrioridad(void)
{
    int opcionVia, opcionPeligro, opcionObstruye, opcionDetenido, puntaje, prioridad; 
    puntaje = 0;
    do{
        printf("En que tipo de via se encuentra? : \n");
        printf("1 - Autopista.\n");
        printf("2 - Ruta.\n");
        printf("3 - Calle.\n");
        printf("Opcion: ");
        scanf("%d",&opcionVia);
        switch (opcionVia)
        {
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
                    printf("Ingrese una opcion valida.\n");
                    break;
        }
    }while(opcionVia < 1 || opcionVia > 3 );
    system("cls");

    do{
        printf("Se encuentra en una zona peligrosa? : \n");
        printf("1 - Si.\n");
        printf("2 - No.\n");
        printf("Opcion: ");
        scanf("%d",&opcionPeligro);
        switch (opcionPeligro)
        {
            case 1: 
                    puntaje = puntaje + 3;
                    break;
            case 2:
                    puntaje;
                    break;
            default: 
                    printf("Ingrese una opcion valida.\n");
                    break;
        }
    }while(opcionPeligro < 1 || opcionPeligro > 2 );
    system("cls");

    do{
        printf("Se encuentra obstruyendo la circulacion? : \n");
        printf("1 - Si.\n");
        printf("2 - No.\n");
        printf("Opcion: ");
        scanf("%d",&opcionObstruye);
        switch (opcionObstruye)
        {
            case 1: 
                    puntaje = puntaje + 2;
                    break;
            case 2:
                    puntaje;
                    break;
            default: 
                    printf("Ingrese una opcion valida.\n");
                    break;
        }
    }while(opcionObstruye < 1 || opcionObstruye > 2 );
    system("cls");

    do{
        printf("Su vehiculo esta totalmente detenido? : \n");
        printf("1 - Si.\n");
        printf("2 - No.\n");
        printf("Opcion: ");
        scanf("%d",&opcionDetenido);
        switch (opcionDetenido)
        {
            case 1: 
                    puntaje = puntaje + 2;
                    break;
            case 2:
                    puntaje;
                    break;
            default: 
                    printf("Ingrese una opcion valida.\n");
                    break;
        }
    }while(opcionDetenido < 1 ||opcionDetenido > 2 );
    system("cls");
    prioridad = puntaje;
    return prioridad;
}

void mostrarPrioridad(int prioridad)
{
    if (prioridad <= 2 ){
        printf("Baja\n");
    }else if (prioridad < 6 ){
            printf("Media\n");
        }else{
            printf("Alta\n");
        }
}



/* MENU CLIENTES OPCION 2 : CONSULTA DE SOLICITUD*/
void consultaSolicitud(int usuarioLogueado,solicitudes_t solicitudes[],int cantSolicitudes,char ubicacion [][TEXTO],char tipoVehiculo[][TEXTO],char situacion[][TEXTO])
{   
    int i;
    bool solicitudEncontrada = false;
    for (i = 0; i < cantSolicitudes ; i++)
    {
        if (solicitudes[i].idCliente==usuarioLogueado)
        {   
            solicitudEncontrada = true;
            printf("-------------------------------------------\n");
            printf("------------Estado de su solicitud---------\n");
            printf("-------------------------------------------\n");
            printf("\n");
            printf("Cliente Id N°: %d \n",solicitudes[i].idCliente);
            printf("-------------------------------------------\n");
            printf("Ubicación: %s\n",ubicacion[solicitudes[i].ubicacion-1]);
            printf("-------------------------------------------\n");
            printf("Tipo de vehiculo: %s\n",tipoVehiculo[solicitudes[i].tipoVehiculo-1]);
            printf("-------------------------------------------\n");
            printf("Situación: %s\n",situacion[solicitudes[i].situacion-1]);
            printf("-------------------------------------------\n");
            printf("Patente: %s\n",solicitudes[i].patente);
            printf("-------------------------------------------\n");
            printf("Estado: \n");
            switch (solicitudes[i].estado)
            {
                case SIN_SOLICITUD:
                    printf("Sin solicitud\n");
                    break;

                case SOLICITUD_PENDIENTE:
                    printf("Pendiente de asignacion\n");
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
        if (solicitudEncontrada == false)
        {
         printf("No realizo ninguna solicitud\n");
        }
}
/* MENU CHOFER */
void menuChofer(int usuarioLogueado,chofer_t choferes[], char ubicacion[][TEXTO], char tiposGrua[][TEXTO]) {
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
                system("cls");
                estadoActual(usuarioLogueado,choferes,ubicacion,tiposGrua);
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
/* MENU CHOFER OPCION 1 : CONSULTA DE ESTADO ACTUAL*/
void estadoActual(int usuarioLogueado,chofer_t choferes[], char ubicacion[][TEXTO], char tiposGrua[][TEXTO])
{
    int opcionEstado, cambiarEstado;
    int i, indiceChofer = -1;
    system("cls");
    printf("---------------------------------------------------------\n");
    printf("                     Estado actual                        \n");
    printf("Por defecto, el sistema le asignara estado: No disponible \n");
	printf( "---------------------------------------------------------\n");
    
    for (i = 0; i < CANT_CHOFERES ; i++)
    {
        if (usuarioLogueado == choferes[i].idPerfil)
        {
            indiceChofer=i;
        }

    }

    if (choferes[indiceChofer].estado == DISPONIBLE) 
    {
        printf("Estado: Disponible\n");
    }else if (choferes[indiceChofer].estado == OCUPADO) 
    {
        printf("Estado: Ocupado\n");
    }else
        {
            printf("Estado: No disponible\n");
        }  
    printf("........................................................\n");
    
    do{
        printf("1 - Cambiar de estado. \n");
        printf("2 - Volver al menu principal\n");
        scanf("%d",&opcionEstado);
        if (opcionEstado > 2 || opcionEstado < 1)
        {   
            system("cls");
            printf("Por favor ingrese una opcion valida.\n");
        }
    }while (opcionEstado > 2 || opcionEstado < 1 );

   if (opcionEstado == 1)
   {
        do{
            system("cls");
            printf("--------------------------------------\n");
            printf("          CAMBIAR ESTADO              \n");
            printf("--------------------------------------\n");
            printf("1. Disponible\n");
            printf("2. No disponible\n");
            printf("3. Volver al menu principal\n");
            printf("Ingrese una opcion: ");
            scanf("%d",&cambiarEstado);
            if(cambiarEstado < 1 || cambiarEstado > 3)
            {
                printf("La opcion ingresada no es valida. Por favor, intente nuevamente\n");
            }
        }while (cambiarEstado < 1 || cambiarEstado > 3 );

        switch (cambiarEstado)
        {
            case 1: 
                    choferes[indiceChofer].estado = DISPONIBLE; 
                    printf("El estado ha sido cambiado. Su estado actual es: Disponible.\n");
                    break;
            case 2:
                    if (choferes[indiceChofer].estado == NO_DISPONIBLE)
                    {
                        printf("Ya se encuentra en estado No Disponible.\n");
                    }else{
                        choferes[indiceChofer].estado = NO_DISPONIBLE;
                        printf("El estado ha sido cambiado. Su estado actual es: No disponible.\n");
                    }
                    break;
            case 3:
                printf("Volviendo al menu anterior...\n");
                Sleep(1500);
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
