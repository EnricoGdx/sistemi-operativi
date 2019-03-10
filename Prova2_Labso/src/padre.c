#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <signal.h>
#include "albero.h"
#include "padre.h"
#include "figlio.h"

#define colore0 "\x1b[31m"
#define colore1 "\x1b[32m"
#define colore2 "\x1b[33m"
#define colore3 "\x1b[34m"
#define colore4 "\x1b[35m"
#define colore5 "\x1b[36m"
#define reset "\x1b[0m"

struct processo* radice;

void pnew(char* nome)
{
	
	if(radice == NULL)
	{
		radice = inizializza();
	}
	struct processo* p;
	p = cerca(nome,radice,0);
	if(p == NULL)
	{
		
		const int STR_LENGTH = 256;
		char nomeFiglio[STR_LENGTH];
		strcpy(nomeFiglio, nome);
	
		struct processo* pfiglio = (struct processo*) malloc(sizeof(struct processo));
		int pid = fork();
		//printf("blabla\n");
		//printf("pid: %d\n", pid);

		
	
		if(pid == 0)
		{
			//printf("mi chiamo: %s", nomeFiglio);
			//printf(" il mio pid: %d", pid);
			printf("\nIl processo %s è stato creato con il pid %d!\n\n", nomeFiglio, getpid());
	
			main_figlio(nomeFiglio);
		}
		else if(pid > 0)
		{
			int fifo = open("tmp/scrittura",O_WRONLY);	

			pfiglio->pid = pid;
			pfiglio->ppid = getpid();
			strcpy(pfiglio->nome, nome);
			pfiglio->padre = radice;
			pfiglio->figlio= NULL;
			pfiglio->fratello = NULL;
			inserisciFiglio(radice, pfiglio);
	
			sleep(1);
		}
		else if(pid < 0)
		{
			printf("\nIl processo %s non è stato creato a causa di un errore!\n", nome);
		}
	}
	else
	{
		printf(colore0 "\nIl processo %s è già stato creato!\n"reset, nome);
	}
}
void plist()
{
	stampaLista(radice);
	printf("\n");
}
void pinfo(char* nome)
{
	struct processo* p = cerca(nome, radice,0);
	if(p == NULL)
	{
		printf(colore0"\nIl processo %s non esiste!\n"reset, nome);
	}
	else
	{
		kill(p->pid, SIGCONT);
		printf("\n");
		char buffer[256];
		strcpy(buffer, "pinfo ");
		strcat(buffer, nome);
		//strcat(buffer, nome);printf("3");
		int fifo = open("tmp/scrittura",O_WRONLY);	
	
		write(fifo, buffer, strlen(buffer));
		close(fifo);
		
		printf("\n");
		sleep(1);
	}

	
}

void prclose(char* nome)
{
	struct processo* p = cerca(nome, radice,1);
	if(p == NULL)
	{
		printf(colore0"\nIl processo %s non esiste!\n"reset, nome);
	}
	else
	{
		kill(p->pid, SIGCONT);
		char buffer[256];
		strcpy(buffer, "pstop ");
		strcat(buffer, nome);
		strcat(buffer, " ");
		char pidc[256];
		char buffertodo [512];
		sprintf(pidc, "%d",  (p->figlio)->pid);
		strcpy(buffertodo,buffer);
		strcat(buffertodo,pidc);
		//strcat(buffer, nome);printf("3");
		
		int fifo = open("tmp/scrittura",O_WRONLY);	
	
		write(fifo, buffertodo, strlen(buffertodo));
		close(fifo);

		sleep(2);

		//write(fifo, buffer, strlen(buffer));
		//close(fifo);
		
		eliminaFiglio(nome, radice);
	
		sleep(1);	
		printf("Il processo %s è stato chiuso!\n", nome);
	}
}
void quit(char* nome)
{
	struct processo* p = cerca(nome, radice,0);
	if(p != NULL)
	{
		if(strcmp(nome, p->nome) == 0)
		{
			struct processo* f = p->figlio;
			if(f != NULL)
			{
				quit(f->nome);
				prmall(f->nome, 1);
			}
		}
		else
		{
			struct processo* f = p->fratello;
			if(f != NULL)
			{
				quit(f->nome);
				prmall(p->nome, 1);
			}
		}
	}
	
}
void pspawn(char* nome)
{
	struct processo* p = cerca(nome, radice,0);
	if(p == NULL)
	{
		printf(colore0"\nNon è possibile clonare il processo %s, poiché non esiste!\n" reset, nome);
	}
	else
	{
		printf("%d\n", p->pid);
		kill(p->pid, SIGCONT);
		
		struct processo* clone = (struct processo*) malloc(sizeof(struct processo));
		char* nomeClone = (char*) malloc(sizeof(nome+257)); 
		char num[256];
		//strcpy(num, "");
		sprintf(num, "%d", p->figli + 1); 
		//printf("\nnum: %s\n", num);
		//strcpy(nomeClone, ""); 
		strcpy(nomeClone, nome);
		strcat(nomeClone, "_"); 
		strcat(nomeClone, num); 

		//printf("\n%s\n", nomeClone);

		strcpy(clone->nome, nomeClone); 
		clone->ppid = p->pid; 
		clone->padre = p; 
		clone->figli = 0;
		clone->figlio = NULL; 
		clone->fratello = NULL; 

		//printf("nome: %s\n", clone->nome);
		
		char buffer[256]; 
		strcpy(buffer, "pspawn ");
		strcat(buffer, nome);
		//strcat(buffer, nome);printf("3");
		
		printf("\n%s\n", buffer);
		int fifo = open("tmp/scrittura",O_WRONLY);
	
		write(fifo, buffer, strlen(buffer));

		close(fifo);
	
		//sleep(3);
		printf("sono dopo lo sleep\n");
		char msg[256];
		strcpy(msg,"");
		fifo = open("tmp/lettura", O_RDONLY);
		
		int length = (int)read(fifo, msg, 256);
		msg[length]='\0';
		printf("%s\n", msg);
		clone->pid = atoi(msg);

		close(fifo);		
		
		inserisciFiglio(p, clone);
		
		sleep(1);
	}
}
void prmall(char* nome, int livello)
{
	struct processo* p = cerca(nome, radice,0);
	if(p == NULL)
	{
		printf(colore0"\nIl processo non esiste\n" reset);
	}
	else
	{
		struct processo* primofiglio = p->figlio;
		if(livello == 1)
		{
			
			if(p->figli > 0)
			{
				prmall(primofiglio->nome, livello + 1);	
			}
			prclose(p->nome);
		}
		else
		{
			struct processo* fratello = p->fratello;
			if(fratello !=NULL)
			{
				prmall(fratello->nome, livello);
			}
			if(primofiglio != NULL)
			{
				prmall(primofiglio->nome, livello + 1);
			}

			prclose(p->nome);
		}
	}
}
void ptree()
{
	if(radice == NULL)
	{
		radice = inizializza();
	}
	stampaLivelli(radice);
	printf("\n");
}
