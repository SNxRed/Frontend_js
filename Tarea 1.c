#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

// Esta tarea se desarrollo con un modelo estatico

int main(){
    
    int pi[2]; // pipes para lectura y escritira 
    int pi2[2]; // 0 se cierra la lectura --- 1 es cierre de escritura 
    int pi3[2]; 
    int pi4[2];


    char entra[] = "Soy Server"; // respuesta del Padre  
    char palabrasH[] = "Soy Hijo"; // Respuesta del hijo 1
    char palabrasCH2[] = "Soy el Clon de Hijo";
    char palabarasN[] = "Soy Nieto";

    pid_t p;
    pid_t p2; // Para el proceso clon 

    pid_t ph2;

    if(pipe(pi) == -1){                 // Esto se utiliza para verificar que las pipes funcionen correctamente
        printf("Error pipe 1");
        return 1;
    }

    if(pipe(pi2) == -1){
        printf("Error pipe 2");
        return 1;
    }

    if(pipe(pi3) == -1){
        printf("Error en la pipe 3");
        return 1;
    }

    if(pipe(pi4) == -1){
        printf("Error pipe 4");
    }
    /*printf("text: ");
    scanf("%s", entra);*/

    p = fork();

    if(p < 0){
        printf("Error en el proceso");
        return 1;
    }

    else if(p > 0){ // PADRE 

        close(pi2[1]); // Termino del proceso Hijo
        read(pi2[0], palabrasH, strlen(palabrasH)); // se lee lo que envia el proceso hijo
        printf("Mensaje del H: %s\n", palabrasH); // Deberia escribir "Soy hijo"
        printf("ID P: %d\n", getpid()); // ID del Padre ya que el es el que recibe el mensaje del proceso Hijo
        close(pi2[0]); 

        close(pi[0]); // Se cierra la lectura de la pipe 1
        write(pi[1], entra, strlen(entra)); // se escribe y envia al proceso hijo
        sleep(1);

        //-------------------------  Clon de Hijo -----------------------------
        // Para esto  se utiliza el mismo metodo que para el clon del hijo 

        close(pi3[1]);
        read(pi3[0], palabrasCH2, strlen(palabrasCH2));
        printf("Mensaje del H2: %s\n", palabrasCH2);
        printf("ID P: %d\n", getpid());
        close(pi3[0]);

        sleep(2);

        close(pi[0]);
        write(pi[1], entra, strlen(entra));
        sleep(1);

        // Conversacion entre Hijo y clon 

       close(pi[0]);
       write(pi[1], palabrasCH2, strlen(palabrasCH2));
       sleep(1);

//---------------- Nieto -----------------------------------
       
        close(pi4[1]); // Termino del proceso Hijo
        read(pi4[0], palabarasN, strlen(palabarasN)); // se lee lo que envia el proceso hijo
        printf("Mensaje del H: %s\n", palabarasN); // Deberia escribir "Soy hijo"
        printf("ID P: %d\n", getpid()); // ID del Padre ya que el es el que recibe el mensaje del proceso Hijo
        close(pi4[0]); 

        close(pi[0]); // Se cierra la lectura de la pipe 1
        write(pi[1], entra, strlen(entra)); // se escribe y envia al proceso hijo
        sleep(1);
       

    }else{ // HIJO
        
        
        close(pi2[0]);// Se cierra la lectura de la pipe 2
        write(pi2[1], palabrasH, strlen(palabrasH)); // Se escribe y envia al proceso padre
        close(pi2[1]); // Se cierra la escritura 

        close(pi[1]);
        read(pi[1], entra, strlen(entra));
        printf("Mensaje del Server: %s\n",entra); // Deberia escribir "Soy server"
        printf("ID H: %d\n", getpid()); // ID del Hijo ya que el es el que recibe el mensaje del Padre 
        close(pi[0]);
        
        sleep(2);

        // Comunicacion con el clon del Hijo 
        // Con esto el proceso padre maneja el mensaje del clon que se le envia al hijo 
        close(pi[1]);
        read(pi[0], palabrasCH2, strlen(palabrasCH2));
        printf("Mensaje de clon H: %s\n",palabrasCH2);
        printf("ID H: %d\n", getpid());
        close(pi[0]);

        sleep(2);

//---------------------- Nieto ---------------------------------
        ph2 = fork(); // creamos un proceso dentro del hijo

        if (fork() == 0){ // trasbajamos con ese preceso  y utilizamos lo mismo que en las comunicaciones anteriores 
            close(pi4[0]);
            write(pi4[1], palabarasN, strlen(palabarasN));
            close(pi4[1]);

            close(pi[1]);
            read(pi[1], entra, strlen(entra));
            printf("Mensaje del Server: %s\n",entra); 
            printf("ID N: %d\n", getpid());  
            close(pi[0]);
        
            sleep(2);

      }

    }

//-------------------------------------------------------------------------------------------------------------------------------------

    p2 = fork(); // Proceso clon 
    

    if(p2 < 0){ // HIJO 2
        printf("Error en el proceso");
        return 1;
    }

// Para crear un clon del Hijo utilizamos otro proceso aparte este toma la id del proceso hijo

    else if( p2 > 0){ // Clon del Hijo
        
        close(pi3[0]);
        write(pi3[1], palabrasCH2, strlen(palabrasCH2));
        close(pi3[1]);

        close(pi[1]);
        read(pi[1], entra, strlen(entra));

        printf("Mensaje del Server: %s\n", entra);
        printf("ID clon H: %d\n", getpid());
        close(pi[0]);

       // sleep(2);

        // Comunicacion con el clon del Hijo

        close(pi3[0]); // Se cierra la lectura de Clon
        write(pi3[1], palabrasCH2, strlen(palabrasCH2)); // Se escribe y envia al Hijo
        close(pi3[1]); 

    }

}