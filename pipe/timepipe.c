#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv){
	struct timeval tv;
	time_t t_inicio;
	time_t t_fin;
	
	char leer_msj[BUFFER_SIZE];
	int fd[2];
	pid_t pid;
	char texto[20];
	int estado;

	//Creacion the pipe
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}

	//Fork a proceso hijo
	pid = fork();
	
	if(pid < 0){
		//En caso de ocurra ERROR
		perror("Fork failed!");
		return 1;
	}
	
	if(pid == 0){
		//Proceso Hijo
		gettimeofday(&tv, NULL);
		t_inicio = tv.tv_usec;

		//Cierro que no se uso del pipe
		close(fd[READ_END]);

		//Escribo en el pipe
		sprintf(texto, "%ld", t_inicio);
		write(fd[WRITE_END], texto, strlen(texto) + 1);

		//Cierro el write del final del pipe
		close (fd[WRITE_END]);
		char *args_b[] = {argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
        	execvp(argv[1], args_b);
        	perror("execvp");
        	exit(1);
	}

	else {
        	//Proceso Padre
        	int w_pid = waitpid(pid, &estado, NULL);
        	if(w_pid == -1){
            		perror("waitpid");
            		exit(1);
        	}
        	gettimeofday(&tv, NULL);
        	t_fin = tv.tv_usec;

        	//Cerrar lo que no se usa del pipe
        	close(fd[WRITE_END]);

        	//Leer desde el pipe
        	read(fd[READ_END], leer_msj, BUFFER_SIZE);
        	fprintf(stdout, "Tiempo Transcurrido: %ld ms\n", t_fin - atoi(leer_msj));

        	//Cierro la lectura del pipe
        	close(fd[READ_END]);
    	}

    return 0;

}
