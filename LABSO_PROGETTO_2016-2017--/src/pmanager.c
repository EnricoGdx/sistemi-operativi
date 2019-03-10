#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "padre.h"
#include "figlio.h"
#include "albero.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define rosso "\x1b[31m"
#define verde "\x1b[32m"
#define giallo "\x1b[33m"
#define blue "\x1b[34m"
#define magenta "\x1b[35m"
#define azzurro "\x1b[36m"
#define reset "\x1b[0m"

#define boldrosso "\033[1m\033[31m"
#define boldblu "\033[1m\033[34m"
#define boldgiallo "\033[1m\033[33m"
#define boldazzurro "\033[1m\033[36m"

void test();
void comandi();
void help();

int main(int argc, char* argv[])
{
	size_t size_ist = 256;
	//char istruzione[(int)size_ist];
	//char buffer[(int)size_ist];
	char* istruzione = (char*)malloc(sizeof(char)*size_ist);
	char* buffer = (char*)malloc(sizeof(char)*size_ist);

	mkfifo("tmp/scrittura", FILE_MODE);
	mkfifo("tmp/lettura", FILE_MODE);

	
	//Avvio del programma in modalità normale
	if(argc == 1)
	{
		help();
		do{
			strcpy(buffer, "");
			strcpy(istruzione, "");
			printf( reset "Inserisci un comando disponibile:\n"); 

			fgets(buffer,size_ist,stdin);
			strncpy(istruzione, buffer, (strlen(buffer)-1));
			istruzione[strlen(buffer)-1] = '\0';
			//printf("%s\n", istruzione);
	
			if(strcmp(istruzione, "") != 0)
			{
				comandi(istruzione);
			}
		}while(strcmp(istruzione, "quit") != 0);
	}
	else if(argc == 2) //Avvio del programma in modalità test
	{
		test(argv[1]);
	}

	unlink("tmp/scrittura");
	unlink("tmp/lettura");

	return(0);
}

//Metodo che legge il file di test
void test(char* file)
{
	printf(verde"Hai avviato la modalità di test!\n");
	printf("%s\n", file);
	FILE* fileTest = fopen(file, "r");
	//printf("File aperto\n");
	
	char istruzione[256];
	char buffer[256];	

	if(fileTest == NULL)
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso" Si è verificato un problema con l'apertura del file di test!\n" reset);
		exit(1);
	}
	
	while(fgets(buffer, 256, fileTest) != NULL)
	{
		strncpy(istruzione, buffer, (strlen(buffer)-1));
		istruzione[strlen(buffer)-1] = '\0';
			
		comandi(istruzione);
		sleep(1);
	}

	fclose(fileTest);
}

void comandi(char* istruzione)//Metodo che interpreta i comandi passati in ingresso
{
	
	char* ist;
	unsigned int i = 0;
			
		//printf("Result from fgets: %s\tLength: %d\n", buffer, (int)strlen(buffer));
		//printf("Istruzione = %s\n", istruzione);
		
		//fgets(istruzione, sizeof(istruzione), stdin);
		//istruzione[strlen(istruzione)] = '\0';
	
		//getline(&istruzione, &size_ist, stdin);
		//if((strlen(buffer)-1) > 1)
		//{
		//}
		//printf("%s", istruzione);
		//strcpy(istruzione, buffer);
		//printf("Istruzione da strtok: %s\n", istruzione);
		//printf("%s", ist);
		//ist = strtok(istruzione, " ");
		//printf("Istruzione: %s\t Length: %d\n",ist, (int)strlen(ist));
	if(strlen(istruzione) < 256)
	{
		if((ist = strtok(istruzione, " ")) != NULL)
		{
			if(strcmp(ist, "phelp") == 0)
			{
				help();
	
			}else if(strcmp(ist, "plist") == 0)
			{
				printf("\nLista dei processi creati:\n");
				plist();
	
			}else if(strcmp(ist, "pnew") == 0)
			{
				//ist = strtok(NULL, " ");
				if((ist = strtok(NULL, " ")) != NULL)
				{
					if(trovaCarattere(ist, '_') == 0)
					{
						pnew(ist);
					}
					else
					{
						printf(boldrosso"\nERROR:"reset);
						printf(rosso" Non è possibile inserire il carattere '_' all'interno del nome del processo\n\n" reset);
					}
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Per creare un processo devi inserire un nome\n\n" reset);
				}
	
			}else if(strcmp(ist, "pinfo") == 0)
			{
				if((ist = strtok(NULL, " ")) != NULL)
				{
					pinfo(ist);
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Per leggere le informazioni di un processo devi inserire un nome\n\n" reset);
				}
	
			}else if(strcmp(ist, "pclose") == 0)
			{
				if((ist = strtok(NULL, " ")) != NULL)
				{
					prclose(ist, 0);
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Per chiudere un processo devi inserire un nome\n\n" reset);
				}
			}else if(strcmp(ist, "quit") == 0)
			{
				printf("\nAvvio della chiusura della applicazione...\n");
				printf("\nL'operazione di chiusura potrebbe richiedere tempo... Per favore, attendere...\n");
				
				quit("shell custom");
				struct processo* radice = prendiRadice();
				elimina(radice);
				
			}else if(strcmp(ist, "pspawn") == 0)
			{
				if((ist = strtok(NULL, " ")) != NULL)
				{
					pspawn(ist);
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Per clonare un processo devi inserire un nome\n\n" reset);
				}
			}else if(strcmp(ist, "prmall") == 0)
			{
				if((ist = strtok(NULL, " ")) != NULL)
				{
					prmall(ist, ist, 0);
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Per chiudere un processo e i suoi cloni devi inserire un nome\n\n" reset);
				}
			}else if(strcmp(ist, "ptree") == 0)
			{
				ptree();
			}
			else if(strcmp(ist, "pchname") == 0)
			{
				if((ist = strtok(NULL, " ")) != NULL)
				{
					char* nome;
					if((nome = strtok(NULL, " ")) != NULL)
					{
						if(trovaCarattere(nome, '_') == 0)
						{
							pchname(ist, nome, 1);
						}
						else
						{
							printf(boldrosso"\nERROR:"reset);
							printf(rosso" Non è possibile inserire il carattere '_' all'interno del nuovo nome del processo\n\n" reset);
						}
					}
					else
					{
						printf(boldrosso"\nERROR:"reset);
						printf(rosso" Per cambiare il nome di un processo esistente devi inserire un nuovo nome\n\n" reset);
					}
				}
				else
				{
					printf(boldrosso"\nERROR:"reset);
					printf(rosso" Per cambiare il nome di un processo processo esistente devi inserire il nome da cambiare\n\n" reset);
				}
			}
			else if(strcmp(ist, "ptreetab") == 0)
			{
				ptreetab();
			}
			else if(strcmp(ist, "ptable") == 0)
			{
				printf(boldblu"\nPID\t\tPPID\t\tSTATO\t\t\tNOME\n" reset);
				ptable();
				printf("\n");
			}
			else{
				printf(boldrosso"\nERROR:"reset);
				printf(rosso " Comando non esistente!\nDigita il comdando \"phelp\" per vedere i comandi disponibili!\n\n" reset);
			}
		}
	}
	else
	{
		printf(boldrosso"\nERROR:"reset);
		printf(rosso " Hai inserito un nome troppo lungo!\n\n" reset);
	}
}	

void help()
{
	printf("\n");
	printf("I comandi disponibili sono:\n");
	printf(boldblu "COMANDI BASE:\n" reset);
	printf("-");
	printf(azzurro " phelp" reset);
	printf(", stampa un elenco di comandi disponibili;\n");
	printf("-");
	printf(azzurro " plist"reset);
	printf(", stampa un elenco di processi generati dalla shell custom;\n");
	printf("-");
	printf(azzurro " pnew <nome>" reset);
	printf(", crea un nuovo processo chiamato <nome>;\n");
	printf("-");
	printf(azzurro" pinfo <nome>" reset);
	printf(", stampa le informazioni relative al processo chiamato <nome>;\n");
	printf("-");
	printf(azzurro " pclose <nome>"reset);
	printf(", chiede al processo chiamato <nome> di chiudersi;\n");
	printf("-");
	printf(azzurro " quit" reset);
	printf(", esce dalla shell custom e chiude tutti i processi attivi;\n");
	printf(boldblu"COMANDI AVANZATI:\n" reset);
	printf("-");
	printf(azzurro " pspawn <nome>" reset);
	printf(", chiede al processo chiamato <nome> di clonarsi, creando un altro processo chiamato <nome_i> con i progressivo;\n");
	printf("-");
	printf(azzurro" prmall <nome>"reset);
	printf(", chiede al processo chiamato <nome> di chiudersi, chiudendo eventuali cloni;\n");
	printf("-");
	printf(azzurro " ptree" reset);
	printf(", mostra la gerarchia completa dei processi generati(SENZA tabulazione);\n");
	printf(boldblu"COMANDI AGGIUNTIVI:\n" reset);
	printf("-");
	printf(azzurro" ptreetab"reset);
	printf(", mostra la gerarchia completa dei processi generati(CON tabulazione);\n");
	printf("-");
	printf(azzurro" pchname <vecchio nome> <nuovo nome>"reset);
	printf(", cambia il nome del processo da <vecchio nome> a <nuovo nome>. Se il processo ha dei cloni, allora anche il loro nome cambia;\n");
	printf("-");
	printf(azzurro" ptable" reset);
	printf(", tabella dettagliata dei processi generati;\n");
	printf("\n");
	printf(reset);
}



