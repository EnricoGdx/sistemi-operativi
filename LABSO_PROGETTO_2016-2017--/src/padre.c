#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <signal.h>
#include <time.h>
#include "albero.h"
#include "padre.h"
#include "figlio.h"


#define rosso     "\033[31m"      
#define verde   "\033[32m"      
#define giallo  "\033[33m"     
#define blu    "\033[34m"      
#define magenta "\033[35m"      
#define azzurro    "\033[36m"  
#define reset   "\033[0m"    

#define boldblu    "\033[1m\033[34m"
#define boldrosso "\033[1m\033[31m"
#define boldverde "\033[1m\033[32m"
#define boldgiallo "\033[1m\033[33m"

struct processo* radice;

void pnew(char* nome)//Metodo che crea un nuovo processo
{
	if(radice == NULL) //Inizializza la radice se è NULL
	{
		radice = inizializza();
	}
	
	struct processo* p = cerca(nome,radice);//cerca nella struttura un processo con lo stesso nome
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
			printf(boldverde "\nSUCCESS: "reset);
			printf(verde"Il processo %s è stato creato con SUCCESSO con il pid %d!\n\n", nome, getpid());
	
			main_figlio(nomeFiglio);
		}
		else if(pid > 0)
		{
			int fifo = open("tmp/scrittura",O_WRONLY); //Apre la fifo, per non dare problemi in futuro

			//Si inizializzano tutte le informazioni riguardo il nuovo processo
			pfiglio->vivo = 1;
			pfiglio->cloni = 0;
			pfiglio->figli = 0;
			pfiglio->pid = pid;
			pfiglio->ppid = getpid();
			strcpy(pfiglio->nome, nome);
			pfiglio->padre = radice;
			pfiglio->figlio= NULL;
			pfiglio->fratello = NULL;
			inserisciFiglio(radice, pfiglio);
	
			usleep(1000);
		}
		else if(pid < 0)
		{
			printf(boldrosso"\nERROR:"reset);
			printf(rosso " Il processo %s non è stato creato a causa di un errore!\n\n" reset , nome);
		}
	}
	else //Se un processo ha lo stesso nome avvisa, non gestisce le omonimie
	{
		printf(boldgiallo "\nWARNING:"reset);
		printf(giallo " Il processo %s è già stato creato in precedenza!\n\n"reset, nome);
	}
}
void plist()
{
	if(radice == NULL) //Inizializza la radice se è NULL
	{
		radice = inizializza();
	}

	stampaLista(radice);
	printf("\n");
}
void pinfo(char* nome)
{
	struct processo* p = cerca(nome, radice); //cerca il processo per recuperare le informazioni
	if(p == NULL)
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso" Il processo %s non esiste!\n\n"reset, nome);
	}
	else
	{
		if(p->vivo == 0)
		{
			printf(boldgiallo "\nWARNING:"reset);
			printf(giallo" Il processo %s è terminato!\n\n"reset, nome);
			return;
		}
		else
		{
			printf("\nInformazioni del processo %s\n", nome);
	
			kill(p->pid, SIGCONT); //Risveglia il processo 
			char buffer[256];
			strcpy(buffer, "pinfo ");
			strcat(buffer, nome);
			//strcat(buffer, nome);printf("3");
			int fifo = open("tmp/scrittura",O_WRONLY);	
		
			write(fifo, buffer, strlen(buffer)); //Si invia un messaggio del comando da eseguire al processo
			close(fifo);

			usleep(100000);

			printf(boldblu "Cloni attivi: "reset);
			printf(blu "%d\n" reset, p->cloni);
			printf(boldblu "Stato: "reset);
			printf(blu "attivo\n\n" reset);
			
			
		}
	}

	
}

void prclose(char* nome, int i) //Metodo che chiude il processo nome
{
	struct processo* p = cerca(nome, radice);
	if(p == NULL)
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso" Il processo %s non esiste!\n\n"reset, nome);
	}
	else
	{
		if(p->vivo == 1 && p->cloni == 0)
		{
			int fifo;
			struct processo* padre = p->padre;
	
			kill(p->pid, SIGCONT);//Sveglia il processo da chiudere
	
			char buffer[256];
			char num[256];
			sprintf(num, "%d", i);
			strcpy(buffer, "pclose ");
			strcat(buffer, nome);
			strcat(buffer, " ");
			strcat(buffer, num);
			//strcat(buffer, nome);printf("3");
			
			fifo = open("tmp/scrittura",O_WRONLY);	
		
			//printf("buffer close: %s\n", buffer);
			write(fifo, buffer, strlen(buffer));//Invia un messaggio di chiusura
			close(fifo);
			
			usleep(1000);
		
			if(padre->vivo == 1){
				if(padre->pid == getpid())
				{
					waitpid(p->pid, NULL, 0);
				}
				else
				{
					kill(padre->pid, SIGCONT);//sveglia il padre del processo da chiudere
				
					//printf("SONO QUI!\n");
					char msg[256];
					char pid[256];
					strcpy(msg, "prmall ");
					strcat(msg, padre->nome);
					strcat(msg, " ");
					sprintf(pid, "%d", p->pid);
					strcat(msg, pid);
		
					fifo = open("tmp/scrittura",O_WRONLY); //Invia un messaggio di attesa per chiudere il processo clone correttamente senza creare zombie
					//printf("messaggio da inviare:%s\n", msg);
					write(fifo, msg, strlen(msg));
					close(fifo);
				}
			}	
			if(i == 0)
			{
				printf(boldverde"\nSUCCESS: "reset);
				printf(verde "Il processo %s è stato chiuso con successo!\n\n" reset, nome);
			}
			eliminaFiglio(nome, radice);
		}
		else if(p->vivo == 0)
		{	
			if(i == 0)
			{
				printf(boldgiallo "\nWARNING:"reset);
				printf(giallo " Non si può chiudere un processo già terminato!\n\n"reset);
			}
		}
		else
		{
			if(i == 0)
			{
				if(p->cloni > 0)
				{
					printf(boldgiallo "\nWARNING:"reset);
					printf(giallo " Il processo ha dei figli, usa il comando \"prmall <nome>\"!\n\n" reset);
				}
			 }
		}
		usleep(1000);
		
	}
}
void quit(char* nome)//Metodo che chiude tutta l'applicazione
{
	struct processo* p = cerca(nome, radice);
	if(p != NULL)
	{
		if(strcmp(nome, "shell custom") == 0)
		{
			struct processo* f = p->figlio;
			if(f != NULL)
			{
				quit(f->nome);
				prmall(f->nome, nome, 1);
			}
		}
		else
		{
			struct processo* fratello = p->fratello;
			if(fratello != NULL)
			{
				quit(fratello->nome);
				prmall(p->nome, nome, 1);
			}			
		}
	}
	
}
void pspawn(char* nome)//Metodo che crea cloni
{
	struct processo* p = cerca(nome, radice); //Cerca nella struttura il processo da clonare
	if(p == NULL)
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso" Non è possibile clonare il processo %s, perchè non esiste!\n\n" reset, nome);
	}
	else
	{
		if(p->vivo == 0){
			printf(boldgiallo "\nWARNING:"reset);
			printf(giallo" Non è possibile clonare il processo %s, perchè è stato chiuso!\n\n" reset, nome);
			return;
		}
		//printf("%d\n", p->pid);
		kill(p->pid, SIGCONT);//Sveglia il processo da clonare
		
		//Viene creato un nuovo nodo con le informazioni del clone
		struct processo* clone = (struct processo*) malloc(sizeof(struct processo));
		char* nomeClone = (char*) malloc(sizeof(nome+257)); 
		char num[256];
		//strcpy(num, "");
		int contatore = p->figli;
		struct processo* tmp;
		do
		{
			strcpy(nomeClone, "");
			contatore++;
			sprintf(num, "%d", contatore); 
			//printf("\nnum: %s\n", num);
			//strcpy(nomeClone, ""); 
			strcpy(nomeClone, nome);
			strcat(nomeClone, "_"); 
			strcat(nomeClone, num);
			tmp = cerca(nomeClone, radice);
		}while(tmp != NULL);

		p->figli = contatore - 1;
		//printf("\n%s\n", nomeClone);
		
		strcpy(clone->nome, nomeClone); 
		clone->vivo = 1;
		clone->ppid = p->pid; 
		clone->padre = p; 
		clone->figli = 0;
		clone->cloni = 0;
		clone->figlio = NULL; 
		clone->fratello = NULL; 

		//printf("nome: %s\n", clone->nome);
		
		char buffer[256]; 
		strcpy(buffer, "pspawn ");
		strcat(buffer, nome);
		strcat(buffer, " ");
		strcat(buffer, nomeClone);
		//strcat(buffer, nome);printf("3");
		
		//printf("\n%s\n", buffer);
		int fifo = open("tmp/scrittura",O_WRONLY);//Si invia il messaggio di clonazione al processo nome
	
		write(fifo, buffer, strlen(buffer));

		close(fifo);
	
		//sleep(3);
		
		char msg[256];
		strcpy(msg,"");
		fifo = open("tmp/lettura", O_RDONLY);//la shell rimane in attesa del pid del clone per aggiungerlo alla struttura 
		
		int length = (int)read(fifo, msg, 256);
		msg[length]='\0';
		//printf("%s\n", msg);
		clone->pid = atoi(msg);

		close(fifo);		
		
		inserisciFiglio(p, clone);
		
		usleep(1000);
	}
}
void prmall(char* nome, char* partenza, int i)//Medoto che chiude i cloni e il processo
{
	//printf("prmall su: %s partito da %s\n", nome, partenza);
	struct processo* p = cerca(nome, radice);
	if(p == NULL)
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso" Il processo non esiste\n\n" reset);
	}
	else
	{
		struct processo* padre = p->padre;
		struct processo* figlio = p->figlio;
		struct processo* f = p->fratello;
		if(padre != NULL)
		{
			if(figlio !=NULL)
			{
				prmall(figlio->nome, partenza, i);
			}
			if(strcmp(nome, partenza) != 0)
			{
				if(f != NULL){
					prmall(f->nome, partenza, i);
				}
				//while(f != NULL)
				//{
				//	prmall(f->nome, partenza, i);
				//	f = f->fratello;
				//}
			}
			//printf("prmall sta chiudendo %s\n",nome);
			prclose(p->nome, i);
		}
		else
		{
			prmall(figlio->nome, partenza, i);
		}
		
	}
}
void ptree()//Metodo che stampa la gerarchia dei processi senza tabulazione
{
	if(radice == NULL)
	{
		radice = inizializza();
	}
	stampaAlbero(radice);
	printf("\n");
}

void pchname(char* vecchio, char* nuovo, int i)//Cambia il nome del processo da vecchio a nuovo
{
	//printf("vecchio: %s - nuovo: %s\n", vecchio, nuovo);
	struct processo* p = cerca(vecchio, radice);
	
	if(p == NULL)
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso" Il processo non esiste\n\n" reset);
	}
	else
	{
		if(i == 1)
		{
			struct processo* q = cerca(nuovo, radice);
			if(q == NULL)
			{
				pchnamesupporto(nuovo, vecchio, p, 1);
			}	
			else
			{
				printf(boldgiallo "\nWARNING:"reset);
				printf(giallo" Esiste già un processo con il nome %s\n\n" reset, nuovo);
			}
		}	
		else
		{
			//printf("Problema sei qui?\n");
			pchnamesupporto(nuovo, vecchio, p, 0);
		}
	}	
}

void pchnamesupporto(char* nuovo, char* vecchio, struct processo* p, int i)
{
	//printf("supporto: vecchio: %s - nuovo: %s\n", vecchio, nuovo);
	if(p-> vivo == 1)
	{	
			//printf("%s è vivo\n", p->nome);
			kill(p->pid, SIGCONT);
			char buffer[256];
			char nome[256];
			char vnome[256];
			strcpy(vnome, vecchio);
			struct processo* figlio = p->figlio;
			struct processo* fratello = p->fratello;
			struct processo* padre = p->padre;
			if(trovaCarattere(vecchio, '_') == 1) //Se il vecchio nome contiene '_' significa che il nome è di un processo clone e bisogna modificare il nuovo nome di conseguenza
			{
					
				char str[256];
				char* n = strtok(vecchio, "_");
				strcpy(str, n);
				strcpy(nome, "");
				strcat(nome, nuovo);
				while(n != NULL )
				{
					if(strcmp(n, str) != 0)
					{	
						strcat(nome, "_");
						strcat(nome, n);
					}
					n = strtok(NULL, "_");
						
				}
				//printf("nome: %s\n", nome);
					strcpy(p->nome, nome);
			}
			else
			{
				strcpy(nome, nuovo);
				strcpy(p->nome, nuovo);
			}
					
			strcpy(buffer, "pchname ");
			strcat(buffer, vnome);
			strcat(buffer, " ");
			strcat(buffer, nome);
			//strcat(buffer, nome);printf("3");
			int fifo = open("tmp/scrittura",O_WRONLY);	
			//printf("msg: %s\n", buffer);
			write(fifo, buffer, strlen(buffer));//Si invia un messaggio al processo per indicargli il nuovo nome che modificherà nel metodo main
			close(fifo);
			
			usleep(100000);
				
			if(figlio != NULL)
			{
				pchname(figlio->nome, nuovo, 0);
			}

			if(i == 0)
			{			
				if(fratello != NULL)
				{
					if(strcmp(padre->nome, "shell custom") != 0)
					{
						pchname(fratello->nome, nuovo, 0);
					}
				}
			}
	}	
	else
	{	
		struct processo* padre = p->padre;
		struct processo* fratello = p->fratello;
		
		if(strcmp(padre->nome, "shell custom") == 0)
		{
			printf(boldgiallo "\nWARNING:"reset);
			printf(giallo" Non è possibile cambiare il nome del processo %s, perchè è terminato!\n\n" reset, vecchio);
		}
		else
		{
			/*char buffer[256];
			char nome[256];
			char vnome[256];
			strcpy(vnome, vecchio);
			if(trovaCarattere(vecchio, '_') == 1) //Se il vecchio nome contiene '_' significa che il nome è di un processo clone e bisogna modificare il nuovo nome di conseguenza
			{
					
				char str[256];
				char* n = strtok(vecchio, "_");
				strcpy(str, n);
				strcpy(nome, "");
				strcat(nome, nuovo);
				while(n != NULL )
				{
					if(strcmp(n, str) != 0)
					{	
						strcat(nome, "_");
						strcat(nome, n);
					}
					n = strtok(NULL, "_");
						
				}
				//printf("nome: %s\n", nome);
					strcpy(p->nome, nome);
			}
			else
			{
				strcpy(nome, nuovo);
				strcpy(p->nome, nuovo);
			}*/
			if(fratello != NULL)
			{
				pchname(fratello->nome, nuovo, 0);
			}
		}
	}
}
int trovaCarattere(char* stringa, char carattere)//Metodo che cerca nella stringa passata in ingresso la presenza del carattere dato
{
	int i = 0;
	do{
		if(stringa[i] == carattere)
		{
			return(1);
		}
		i++;

	}while(stringa[i] != '\0');

	return(0);
}

void ptreetab()//stampa la gerarchia di processi con tabulazione
{
	if(radice == NULL)
	{
		radice = inizializza();
	}
	stampaLivelli(radice);
	printf("\n");
}

void ptable()//Stampa una tabella con le informazioni di tutti i processi
{
	if(radice == NULL)
	{
		radice = inizializza();
	}
	stampaListaDettagliata(radice);
}

struct processo* prendiRadice()
{
	if(radice == NULL)
	{
		radice = inizializza();
	}
	return(radice);
}
