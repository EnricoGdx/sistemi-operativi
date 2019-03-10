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

/*#define colore0 "\x1b[31m"
#define colore1 "\x1b[32m"
#define colore2 "\x1b[33m"
#define colore3 "\x1b[34m"
#define colore4 "\x1b[35m"*/
#define colore5 "\x1b[36m"
#define reset "\x1b[0m"

struct processo* inizializza()
{
	struct processo* radice = (struct processo*) malloc(sizeof(struct processo));
	radice->pid = getpid();
	radice->figli = 0;
	radice->padre = NULL;
	radice->fratello = NULL;
	radice->figlio = NULL;
	strcpy(radice->nome, "shell costum");
	return radice;
}

void inserisciFiglio(struct processo* padre, struct processo* figlio)
{
	if(padre->figlio == NULL)
	{
		padre->figlio = figlio;
	}
	else
	{
		struct processo* supporto = padre->figlio;
		while(supporto->fratello != NULL)
		{
			supporto =  supporto->fratello;
		}
	
		supporto->fratello = figlio;
	}

	padre->figli = padre->figli+1;
}

void eliminaFiglio(char* nome, struct processo* radice)
{
	struct processo* p = cerca(nome, radice, 0);
	struct processo* padre = p->padre;

	struct processo* f = padre->figlio;
	if(f == p)//SE PRIMO FIGLIO
	{
		padre->figlio = p->fratello;
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
	
}

struct processo* cerca(char* nome, struct processo* p, int padre)
{
	if(p == NULL)
	{
		return(NULL);
	}
	if(strcmp(p->nome, nome) == 0 &&  padre == 0)
	{
		return(p);
	}
	else if(p->figlio != NULL && padre == 1){
		if(strcmp((p->figlio)->nome, nome) == 0){
			return (p);
		} 
	}
	struct processo* ris = cerca(nome, p->figlio, padre);
	if(ris == NULL)
	{
		ris = cerca(nome, p->fratello, padre);
	}
	return(ris);
	
}


void stampaSpazi() {

	int i = 0;	
	while(i < d)
	{	
		//printf("\t");
		printf("  ");
		i++;
	}	
		
}

/*void stampaAlbero(struct processo* albero)
{
 	if(albero != NULL)
	{
		if(strcmp(albero->nome, "shell costum") != 0)
		{
			struct processo* supporto = albero->padre;
			if(supporto->figlio == albero )
			{
				//stampaSpazi();
				stampaAlbero(albero->fratello);
				printf(" %s\n\n", albero->nome);
				d--;
				stampaAlbero(albero->figlio);
				d++;
			}
			else
			{
				//stampaSpazi();
				stampaAlbero(albero->fratello);
				printf(" %s ", albero->nome);
				d--;
				stampaAlbero(albero->figlio);
				d++;
			}
		
		}
		else
		{
			stampaSpazi();
			printf("%s\n\n", albero->nome);
			d--;
			stampaAlbero(albero->figlio);
		}
	}

  /*if (!emptyp(t)) {
    print_indented(t->right);
    print_spaces(depth);
    cout << t->item << endl;
    print_indented(t->left);
  }
  depth--;

}*/

void stampaLivelli(struct processo* albero)
{
	if(albero != NULL)
	{
		if(strcmp(albero->nome, "shell costum") != 0)
		{
			stampaSpazi();
			strcpy(colore, "\x1b[3");
			strcpy(nColore, "");
			sprintf(nColore, "%d", (d%5) + 1);
			strcat(colore, nColore);
			strcat(colore, "m");

			printf("%sLIVELLO %d: %s - %d\n" reset,colore, d, albero->nome, albero->pid);
			d++;
			stampaLivelli(albero->figlio);
			d--;
			stampaLivelli(albero->fratello);
		}
		else
		{
			printf(colore5 "\nLIVELLO %d: %s - %d\n", d, albero->nome, albero->pid);
			d++;
			stampaLivelli(albero->figlio);
			d--;
		}
	}
	
	printf(reset);
}



void stampaLista(struct processo* p)
{
	if(p == NULL)
	{
		return;
	}
	else
	{
		if(p->padre != NULL)
		{
			printf("- processo: %s\n", p->nome);
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
