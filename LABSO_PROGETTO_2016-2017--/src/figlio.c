#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/stat.h> 
#include "figlio.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define rosso "\x1b[31m"
#define verde "\x1b[32m"
#define giallo "\x1b[33m"
#define blu "\x1b[34m"
#define magenta "\x1b[35m"
#define azzurro "\x1b[36m"
#define reset "\x1b[0m"

#define boldrosso   "\033[1m\033[31m"
#define boldverde    "\033[1m\033[32m"
#define boldblu    "\033[1m\033[34m"

const int STR_LENGTH = 256;

/*void handler(int sig)
{
	printf("ATTIVAZIONE!\n");
	fprintf(stderr, "attivazione\n");
}*/

void main_figlio(char* nome) //Metodo principale che eseguono sia i figli della shell che i cloni
{
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
	
	int fifo = open("tmp/scrittura", O_RDONLY); //Apre in lettura la fifo
	//printf("fifo\n");
	int chiusura = 0;
	
	//signal(SIGCONT, handler);

	do{
		kill(getpid(), SIGSTOP); //Si ferma, finchè la shell non lo risveglia
		char* ist;
		int length = (int)read(fifo, msg, STR_LENGTH);//legge il messaggio che ha inviato la shell
		msg[length]='\0';
		//printf("msg: %s - proc: %s\n", msg, nome);
		ist = strtok(msg, " ");
		//printf("mess:%s\n", ist);
		//printf("LEGGO: %s\t length: %d\n", msg, (int)strlen(msg));
		if(strcmp(ist, "pspawn") == 0) //Entra se il messaggio si riferisce alla clonazione
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0) 
			{
				printf(reset"\nClonazione del processo %s con pid %d in corso...\n" reset, nome, getpid());
				if((ist = strtok(NULL, " ")) != NULL)
				{
					/*char num[STR_LENGTH];
					sprintf(num, "%d", i); //In base al nome del processo, si "calcola" il nome da dare al clone
					strcpy(buffer, nome);
					strcat(buffer, "_");
					strcat(buffer, num);*/
					//printf("nomeClone: %s\n", ist);
					clonazione(ist); //esegue la clonazione
				}
				//i++;
			}
		}
		else if(strcmp(ist, "pclose")== 0)//Entra se il messaggio si riferisce alla chiusura del processo stesso
		{
			//printf("PROCESSO DA CHIUDERE: %s\n", nome);
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				ist = strtok(NULL, " ");
				if((atoi(ist)) == 0)
				{
					printf(reset"\nIl processo %s con pid %d si sta chiudendo... Attendere\n"reset, nome, getpid());
				}
				chiusura = 1;
			}
		}
		else if(strcmp(ist, "prmall") == 0) //Entra se il messaggio si riferisce alla chiusura di un clone 
		{
			
			ist = strtok(NULL, " ");
			//printf("%s\n", ist);
			if(strcmp(ist, nome) == 0)
			{
				//printf("nome: %s\n", ist);
				ist = strtok(NULL, " ");
				//printf("pidf: %s\n", ist);
				waitpid(atoi(ist), NULL, 0);//Attente la chiusura del clone con pid ist
			}
		}
		else if(strcmp(ist, "pinfo") == 0)//Entra se il messaggio si riferisce a dare informazioni del processo
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				printf(boldblu"Nome: " reset);
				printf(blu "%s\n" reset, nome);
				printf(boldblu"Pid: "reset);
				printf( blu "%d\n" reset, getpid());
				printf(boldblu"Ppid: "reset);
				printf(blu "%d\n" reset, getppid());
				printf(boldblu"Pgid: "reset);
				printf(blu "%d\n" reset, getpgid(getpid()));
				printf(reset);
			}
		}
		else if(strcmp(ist, "pchname") == 0)//Entra se il messaggio indica l'operazione da eseguir eè il cambio nome
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, nome) == 0)
			{
				if((ist =  strtok(NULL, " ")) != NULL)
				{
					printf(reset"\nCambio del nome %s in ", nome);
					strcpy(nome, ist);
					printf("%s in corso... Attendere\n\n" reset, nome);
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Operazione non riuscita\n\n" reset);
				}
				
			
			}
		}
	}while(!chiusura);
		
	kill(getpid(), SIGINT); //segnale che uccide il processo solo se prima ha ricevuto un messaggio pclose
}


void clonazione(char* nome) //Metodo di clonazione, prendi in ingresso il nome del processo da clonare
{
	int fifo = open("tmp/lettura", O_WRONLY); //Apre una fifo in lettura 
	char buffer[STR_LENGTH]; 
	int pid = fork();
	if(pid == 0)
	{
		printf(boldverde"\nSUCCESS: " reset);
		printf(verde"Il clone %s è stato creato con SUCCESSO con il pid %d!\n\n"reset, nome, getpid());
		sprintf(buffer, "%d", getpid());
		write(fifo, buffer, strlen(buffer)); //Scrive alla shell il pid del clone, in modo da segnarlo nella struttura
		close(fifo);
		main_figlio(nome); //Avvia il processo clone
	}
	else if(pid < 1)
	{
		printf(boldrosso"ERROR: " reset);
		printf(rosso "\nIl processo clone %s non è stato creato a causa di un errore!\n" reset, nome);
	}
}
