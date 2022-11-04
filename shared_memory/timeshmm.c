#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 

int main(int argc, char **argv)
{
	pid_t pid;
	int estado;

	struct timeval tv;
	time_t t_inicio;
	time_t t_fin;

	//El tamaño en bytes del objeto de memoria compartido
	const int SIZE = 4096;

	//Nombre del objeto de memoria compartida
	const char* nombre = "OS_pa2";
	
	//Memoria compartida del archivo descriptor
	int shm_fd;

	//Puntero para objeto de memoria compartido
	void* puntero;

	//Creo el objeto de memoria compartido
	shm_fd = shm_open(nombre, O_CREAT | O_RDWR, 0666);

	//Configuro el tamaño del objeto de memoria compartida
	ftruncate(shm_fd, SIZE);

	//Mapa de memoria compartido con objeto de memoria
	puntero = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	//Fork
	pid = fork();

	if(pid < 0){
		//En caso de Error
		fprintf(stderr, "Fallo el fork");
		return 1;
	}
	else if (pid == 0){
		char texto[20];

		gettimeofday(&tv, NULL);
		t_inicio = tv.tv_usec;

		sprintf(texto, "%ld", t_inicio);
		sprintf(puntero, "%ld", t_inicio);

		puntero += strlen(texto);
		char *args_b[] = {argv[1], argv[2], argv[3], argv[4], argv[5], NULL};
       		 execvp(argv[1], args_b);
        	perror("execvp");
        	exit(EXIT_FAILURE);
	}
	else {
		//Proceso Padre
		//El proceso padre esperara hasta que el proceso hijo se complete
		waitpid(pid, &estado, NULL);

		//El tamaño en bytes del objeto de memoria compartido
		const int SIZE = 4096;

		//Nombre del objeto de memoria compartida
		const char* nombre = "OS_pa2";
	
		//Memoria compartida del archivo descriptor
		int shm_fd;

		//Puntero para objeto de memoria compartido
		void* puntero;

		//Creo el objeto de memoria compartido
		shm_fd = shm_open(nombre, O_RDONLY, 0666);

		//Mapa de memoria compartido con objeto de memoria
		puntero = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
		
		//Leer desde el objeto de memoria compartida
		char *lector = (char*) puntero;
		
		gettimeofday(&tv, NULL);
		t_inicio = tv.tv_usec;

		fprintf(stdout, "Tiempo transcurrido: %ld\n", t_inicio - atoi(lector));
		
		//Remuevo el objeto de memoria compartido
		shm_unlink(nombre);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_SUCCESS);
}
