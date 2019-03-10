#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/stat.h> 
#include "figlio.h"
#include <signal.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

const int STR_LENGTH = 256;

/*void handler(int sig)
{
	printf("ATTIVAZIONE!\n");
	fprintf(stderr, "attivazione\n");
}*/

void main_figlio(char* nome)
{
	printf("sono il figlio\n");
	int i = 1;
	char nomeProcesso[STR_LENGTH];
	char nomeFifo[STR_LENGTH];
	char msg[STR_LENGTH];
	char buffer[STR_LENGTH];
	//strcpy(nomeProcesso, nome);
	//strcpy(nomeFifo, "tmp/");
	strcpy(msg,"");
	//printf("NOME PROCESSO: %s\n", nome);
	//strcat(nomeFifo, nome);
	//strcat(nomeFifo, ".fifo");
	//mkfifo(nomeFifo, FILE_MODE);
	
	int fifo = open("tmp/scrittura", O_RDONLY);
	//printf("fifo\n");
	int chiusura = 0;
	
	//signal(SIGCONT, handler);

	do{
		printf("%s:%d\n", nome, getpid());
		kill(getpid(), SIGSTOP);
		printf("partito\n");
		char* ist;
		int length = (int)read(fifo, msg, STR_LENGTH);
		msg[length]='\0';
		ist = strtok(msg, " ");
		printf("mess:%s\n", ist);
		//printf("LEGGO: %s\t length: %d\n", msg, (int)strlen(msg));
		if(strcmp(ist, "pspawn") == 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				printf("\nClonazione del processo %s con pid %d in corso...\n", nome, getpid());
				char num[STR_LENGTH];
				sprintf(num, "%d", i);
				strcpy(buffer, nome);
				strcat(buffer, "_");
				strcat(buffer, num);
				
				clonazione(buffer);
				i++;
			}
		}
		else if(strcmp(ist, "pclose")== 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				chiusura = 1;
				printf("Il processo %s con pid %d si sta chiudendo!\n", nome, getpid());
			}
		}
		else if(strcmp(ist, "prmall") == 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				length = (int)read(fifo, msg, STR_LENGTH);
				msg[length]='\0';
				waitpid(atoi(msg), NULL, 0);
			}
		}
		else if(strcmp(ist, "pinfo") == 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				printf("Nome: %s\n", nome);
				printf("Pid: %d\n", getpid());
				printf("Ppid: %d\n", getppid());
				printf("Pgid: %d\n", getpgid(getpid()));
			}
		}
		else if(strcmp(ist, "pstop") == 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{	
				printf("Sto chiudendo\n");
				ist = strtok(NULL, " ");
				int pidwait = atoi(ist);
				waitpid(pidwait,NULL,WNOHANG);
			}
		}
		else if(strcmp(ist, "pfinalize") == 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
			}
		}
	}while(!chiusura);
	kill(getpid(), SIGINT);
}


void clonazione(char* nome)
{
	int fifo = open("tmp/lettura", O_WRONLY);
	char buffer[STR_LENGTH]; 
	int pid = fork();
	if(pid == 0)
	{
		printf("\nIl clone %s è stato creato con pid %d!\n", nome, getpid());
		sprintf(buffer, "%d", getpid());
		write(fifo, buffer, strlen(buffer));
		close(fifo);
		main_figlio(nome);
	}
	else if(pid < 1)
	{
		printf("\nIl processo clone %s non è stato creato a causa di un errore!\n", nome);
	}
}
