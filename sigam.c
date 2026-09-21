#include <stdbool.h>  // permite usar un dato booleano
#include <stdio.h>    // IMP, entrada y salida de datos
#include <stdlib.h>   // funciones generales
#include <string.h>   // Trabaja con char
#include <time.h>     // se usa para aleatorios tambien
#include <windows.h> //lo usamos para esperar x cantidad de tiempo

#define CANTCHOFERES 3
#define CANTCLIENTES 3
#define CANTADMIN 1
#define CANTSOPORTE 1
#define TEXTO 30
//Definicion de las estructuras de los perfiles
typedef struct{
    char usuario[TEXTO];
    char clave[TEXTO];
}chofer_t;

typedef struct{
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

//Declaracion de los prototipos
void mostrarBienvenida();
void inicioSesion(char usuario[], char password[]);
char validaLogin (char usuario[], char password[],chofer_t chofer[],cliente_t cliente[],admin_t admin[],soporte_t soporte[],int *choferLogueado, int *clienteLogueado);
//Inicio del algoritmo
int main() { 
    
    //Declaracion de los vectores de las estructuras
    chofer_t chofer[CANTCHOFERES];
    cliente_t cliente[CANTCLIENTES];
    admin_t admin[CANTADMIN];
    soporte_t soporte[CANTSOPORTE];
    
    //Credenciales hardcodeadas
    //strcpy() viene de <string.h> copia una cadena dentro de un campo char[] de la estructura
    strcpy(chofer[0].usuario, "chofer1");
    strcpy(chofer[0].clave,"chofer123");

    strcpy(chofer[1].usuario, "chofer2");
    strcpy(chofer[1].clave,"chofer123");

    strcpy(chofer[2].usuario, "chofer3");
    strcpy(chofer[2].clave,"chofer123");
    //--------------------------------------------
    strcpy(cliente[0].usuario, "cliente1");
    strcpy(cliente[0].clave,"cliente123");

    strcpy(cliente[1].usuario, "cliente2");
    strcpy(cliente[1].clave,"cliente123");

    strcpy(cliente[2].usuario, "cliente3");
    strcpy(cliente[2].clave,"cliente123");
    //--------------------------------------------
    strcpy(admin[0].usuario, "admin1");
    strcpy(admin[0].clave,"admin123");
    //--------------------------------------------
    strcpy(soporte[0].usuario, "soporte1");
    strcpy(soporte[0].clave,"soporte123");

    char usuario[50], password[50];
    char tipoUsuario, valorUsuario;
    int choferLogueado = -1, clienteLogueado = -1;

    //system("pause"); equivalente a esperar tecla
    //system("cls"); equivalente a limpiar pantalla
    //Sleep(3000); // 3 segundos   este es para esperar X segundos, se escribe en milisegundos
 
    valorUsuario='I';

    while(valorUsuario=='I'){ 
        system("cls");
        //system("cls"); equivalente a limpiar pantalla
        mostrarBienvenida();

        // Inicio de sesión
        inicioSesion(usuario, password);

        //Validacion para credenciales incorrectas en login
        valorUsuario=validaLogin(usuario,password,chofer,cliente,admin,soporte,&choferLogueado,&clienteLogueado);
        
        if (valorUsuario=='I'){
            printf("Usuario o clave incorrecta. Por favor, intente nuevamente.\n");
            Sleep(2500);
            system("cls");
        }
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
//Aclaraciones: la funcion devuelve una letra porque la comparacion entre cadenas no es igual que en pseint. 
//strcmp sirve para comparar dos strings.
// usuario y password se comparan sin [] porque eso devolveria una sola letra justamente por lo dicho arriba 
//Incorpore clienteLogueado asi tambien sabemos que cliente es
//choferes devuelve H asi no se pisa con clientes que tambien devolveria C
char validaLogin(char usuario[], char password[],chofer_t chofer[],cliente_t cliente[],admin_t admin[],soporte_t soporte[],int *choferLogueado, int *clienteLogueado){
    int i=0;
    char tipoUsuario='I';
    
    while (i<CANTCHOFERES && tipoUsuario== 'I'){
        if(strcmp(usuario, chofer[i].usuario)==0 && strcmp(password,chofer[i].clave)==0){
            tipoUsuario= 'H';
            *choferLogueado=i;
        }
        i++;
    }
    i=0;
    while (i<CANTCLIENTES && tipoUsuario== 'I'){
        if(strcmp(usuario, cliente[i].usuario)==0 && strcmp(password,cliente[i].clave)==0){
            tipoUsuario= 'C';
            *clienteLogueado=i;
        }
        i++;
    }
    i=0;
    while (i<CANTADMIN && tipoUsuario== 'I'){
        if(strcmp(usuario, admin[i].usuario)==0 && strcmp(password,admin[i].clave)==0){
            tipoUsuario= 'A';
        }
        i++;
    }
    i=0;
    while (i<CANTSOPORTE && tipoUsuario== 'I'){
        if(strcmp(usuario, soporte[i].usuario)==0 && strcmp(password,soporte[i].clave)==0){
            tipoUsuario= 'S';
        }
        i++;
    }
    return tipoUsuario;
} 