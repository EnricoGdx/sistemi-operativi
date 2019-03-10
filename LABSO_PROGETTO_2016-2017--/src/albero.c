#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "albero.h"

int d = 0;
char nColore[256];
char colore[256];

#define rosso "\x1b[31m"
#define verde "\x1b[32m"
#define giallo 1"\x1b[33m"
#define blu "\x1b[34m"
#define magenta "\x1b[35m"
#define azzurro "\x1b[36m"
#define reset "\x1b[0m"

#define boldrosso "\033[1m\033[31m"
#define boldverde "\033[1m\033[32m"
#define boldblu "\033[1m\033[34m"

struct processo* inizializza() //Inizializza il nodo radice e rappresenta la shell custom
{
	struct processo* radice = (struct processo*) malloc(sizeof(struct processo));
	radice->vivo = 1;
	radice->visitato = 0;
	radice->pid = getpid();
	radice->cloni = 0;
	radice->figli = 0;
	radice->padre = NULL;
	radice->fratello = NULL;
	radice->figlio = NULL;
	strcpy(radice->nome, "shell custom");
	return radice;
}

void inserisciFiglio(struct processo* padre, struct processo* figlio) //Viene inserito un nuovo nodo figlio, dando anche il padre in ingresso
{
	if(padre->figlio == NULL) // Se il nuovo nodo è il primo figlio del padre, allora il puntato figlio del padre punterà dirattamente al nodo figlio
	{
		padre->figlio = figlio;
	}
	else //Altrimenti significa che il nuovo nodo non è il primo figlio del nodo padre dato in ingresso
	{
		struct processo* supporto = padre->figlio; 
		while(supporto->fratello != NULL) //Ciclo che cerca l'ultimo figlio del padre
		{
			supporto =  supporto->fratello;
		}
	
		supporto->fratello = figlio; 
	}

	padre->cloni =padre->cloni+1;
	padre->figli = padre->figli+1; //Aumenta il numero di figli
}

void eliminaFiglio(char* nome, struct processo* radice)
{
	
	struct processo* p = cerca(nome, radice); //Cerca il nodo da eliminare
	if(p!= NULL)
	{
		struct processo* padre = p->padre;
		padre->cloni = padre->cloni - 1;
		p->vivo = 0;
	}
	// Il nodo non viene eliminato dalla struttura, ma viene segnato come processo morto

	/*if(p==NULL){
		printf("Nodo non trovato");
		return;
	}
	struct processo* padre = p->padre;

	struct processo* f = padre->figlio;

	printf("da eliminare: %s", p->nome);

	if(f == p)//SE PRIMO FIGLIO
	{
		p->vivo = 0;
		//padre->figlio = p->fratello;
	}
	else
	{
		while(f->fratello != p)
		{
			f = f->fratello;
		}
		
		f->fratello = p->fratello;
	}	
	
	inserisciFiglio(padre, p->figlio);
	padre->figli = padre->figli -1;
	free(p);
	
	printf("figlio eliminato\n");*/
}

struct processo* cerca(char* nome, struct processo* p) //Cerca un nodo dato il nome e il punto iniziale da dove cercare
{
	if(p == NULL)
	{
		return(NULL);
	}
	if(strcmp(p->nome, nome) == 0) //Se il nome corrisponde a quello del nodo dato in ingresso ritorna il nodo
	{
		return(p);
	}
	else //Altrimenti si cerca il nodo tra i figli e i fratelli
	{
		struct processo* ris = cerca(nome, p->figlio); 
		if(ris == NULL)
		{
			ris = cerca(nome, p->fratello);
		}

		return(ris);
	}
}

void stampaSpazi() //Stampa gli spazi per la tabulazione dell'albero
{

	int i = 0;	
	while(i < d)
	{	
		//printf("\t");
		printf("  ");
		i++;
	}	
		
}

void stampaAlbero(struct processo* albero)//mostra la gerarchia di processi senza tabulazione
{
 	if(albero != NULL)
	{
		if(strcmp(albero->nome, "shell custom") != 0) //Se il nodo passato in ingresso non è la shell, allora si verifica se è attivo o terminato
		{
			if(albero-> vivo == 0)
			{
				printf("LIVELLO %d: %s - TERMINATO\n" , d, albero->nome);
				d++;
				stampaAlbero(albero->figlio);
				d--;
				stampaAlbero(albero->fratello);
			}
			else
			{
				strcpy(colore, "\033[3");
				strcpy(nColore, "");
				sprintf(nColore, "%d", (d%5) + 1);
				strcat(colore, nColore);
				strcat(colore, "m");

				printf("%sLIVELLO %d: %s - %d\n" reset,colore, d, albero->nome, albero->pid);
				d++;
				stampaAlbero(albero->figlio);
				d--;
				stampaAlbero(albero->fratello);
			}
		}
		else
		{
			printf(boldblu "\nLIVELLO %d: %s - %d\n" reset , d, albero->nome, albero->pid);
			d++;
			stampaAlbero(albero->figlio);
			d--;
		}
	}
	
	printf(reset);
}

void stampaLivelli(struct processo* albero) //Stampa la gerarchia dei processi con la tabulazione
{
	if(albero != NULL)
	{
		if(strcmp(albero->nome, "shell custom") != 0)
		{
			if(albero-> vivo == 0)
			{
				stampaSpazi();
				printf("LIVELLO %d: %s - TERMINATO\n" , d, albero->nome);
				d++;
				stampaLivelli(albero->figlio);
				d--;
				stampaLivelli(albero->fratello);
			}
			else
			{
				stampaSpazi();
				strcpy(colore, "\033[3");
				strcpy(nColore, "");
				sprintf(nColore, "%d", (d%6) + 1);
				strcat(colore, nColore);
				strcat(colore, "m");

				printf("%sLIVELLO %d: %s - %d\n" reset,colore, d, albero->nome, albero->pid);
				d++;
				stampaLivelli(albero->figlio);
				d--;
				stampaLivelli(albero->fratello);
			}
		}
		else
		{
			printf(boldblu "\nLIVELLO %d: %s - %d\n" reset , d, albero->nome, albero->pid);
			d++;
			stampaLivelli(albero->figlio);
			d--;
		}
	}
	
	printf(reset);
}



void stampaLista(struct processo* p) //stampa una lista di processi
{
	if(p == NULL)
	{
		return;
	}
	else
	{
		if(p->padre != NULL)
		{
			if(p->vivo == 1)
			{
				printf("- processo: ");
				printf(boldverde"%s\n" reset, p->nome);	
			}
			else
			{
				printf("- processo: ");
				printf(boldrosso"%s - TERMINATO\n" reset , p->nome);
			}
			stampaLista(p->fratello);
			stampaLista(p->figlio);
		}
		else
		{
			stampaLista(p->fratello);
			stampaLista(p->figlio);	
		}
	}	
}

void stampaListaDettagliata(struct processo* p) // Stampa una tabella/lista dei processi 
{
	if(p == NULL)
	{
		return;
	}
	else
	{
		if(p->padre != NULL)
		{
			if(p->vivo == 1)
			{
				printf(verde"%d\t\t%d\t\tATTIVO\t\t\t%s\n" reset, p->pid, p->ppid, p->nome);	
			}
			else
			{
				printf(rosso"-\t\t-\t\tTERMINATO\t\t%s\n" reset, p->nome);
			}
			stampaListaDettagliata(p->fratello);
			stampaListaDettagliata(p->figlio);
		}
		else
		{
			stampaListaDettagliata(p->fratello);
			stampaListaDettagliata(p->figlio);	
		}
	}	
}


void elimina(struct processo* p)
{
	if(p != NULL)
	{
		if(strcmp(p->nome, "shell custom") == 0)
		{
			elimina(p->figlio);
		}
		else
		{
			elimina(p->fratello);
			elimina(p->figlio);
		}

		free(p);
	}
}

/*int ricerca(int contatore, char* nome, struct processo* p)
{
	struct processo* tmp = cerca(nome, p);
	if(p == NULL)
	{
		return(contatore);
	}
	else
	{
		if(visitato == 0)
		{
			char* str = strtok(nome, '_');
			while(str != NULL)
			{
				str = strtok(NULL, '_');
			}	
		
			return(
		}
		else
		{
			
			ricerca(contatore+1, , p);
		}
	}
}*/
