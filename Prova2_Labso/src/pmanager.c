#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "padre.h"
#include "figlio.h"
#include <signal.h>
#include <fcntl.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define colore0 "\x1b[31m"
#define colore1 "\x1b[32m"
#define colore2 "\x1b[33m"
#define colore3 "\x1b[34m"
#define colore4 "\x1b[35m"
#define colore5 "\x1b[36m"
#define reset "\x1b[0m"

void test();
void comandi();
void help();

int main(int argc, char** argv[])
{
	size_t size_ist = 256;
	//char istruzione[(int)size_ist];
	//char buffer[(int)size_ist];
	char* istruzione = (char*)malloc(sizeof(char)*size_ist);
	char* buffer = (char*)malloc(sizeof(char)*size_ist);

	mkfifo("tmp/scrittura", FILE_MODE);
	mkfifo("tmp/lettura", FILE_MODE);

	if(argc == 1)
	{
		do{
			strcpy(buffer, "");
			strcpy(istruzione, "");
			printf("Inserisci un comando disponibile:\n"); 

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
	else if(argc == 2)
	{
		test(argv[1]);
	}

	return(0);
}

void test(char* file)
{
	printf("Hai avviato la modalità di test!\n");
	printf("%s\n", file);
	FILE* fileTest = fopen(file, "r");
	//printf("File aperto\n");
	
	char istruzione[256];
	char buffer[256];	

	if(fileTest == NULL)
	{
		printf(colore0"Si è verificato un problema con l'apertura del file di test!\n" reset);
		exit(1);
	}
	
	while(fgets(buffer, 256, fileTest) != NULL)
	{
		strncpy(istruzione, buffer, (strlen(buffer)-1));
		istruzione[strlen(buffer)-1] = '\0';
			
		comandi(istruzione);
	}

	fclose(fileTest);
}

void comandi(char* istruzione)
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
		printf("%s", ist);
		ist = strtok(istruzione, " ");
		//printf("Istruzione: %s\t Length: %d\n",ist, (int)strlen(ist));
		

		if(strcmp(ist, "phelp") == 0)
		{
			help();

		}else if(strcmp(ist, "plist") == 0)
		{
			printf("\nLista dei processi creati:\n");
			plist();

		}else if(strcmp(ist, "pnew") == 0)
		{
			ist = strtok(NULL, " ");
			if(strcmp(ist, "") != 0)
			{
				pnew(ist);
			}
			else
			{
				printf("\nIl comando pnew necessita del <nome> per creare un processo!\n");
			}

		}else if(strcmp(ist, "pinfo") == 0)
		{
			ist = strtok(NULL, " ");
			pinfo(ist);

		}else if(strcmp(ist, "pclose") == 0)
		{
			ist = strtok(NULL, " ");
			prclose(ist);
		}else if(strcmp(ist, "quit") == 0)
		{
			quit("shell costum");
			
		}else if(strcmp(ist, "pspawn") == 0)
		{
			printf("entro in spawn\n");
			ist = strtok(NULL, " ");
			pspawn(ist);
		}else if(strcmp(ist, "prmall") == 0)
		{
			ist = strtok(NULL, " ");
			prmall(ist, 1);
		}else if(strcmp(ist, "ptree") == 0)
		{
			ptree();
		}
		else if(strcmp(ist, "ps") == 0)
		{
			system("ps");
		}
		else{
			printf(colore0 "\nComando non esistente!\nDigita il comdando \"phelp\" per vedere i comandi disponibili!\n\n" reset);
		}

}

void help()
{
	printf(colore2);
	printf("\n");
	printf("I comandi disponibili sono:\n");
	printf("COMANDI BASE:\n");
	printf("- phelp, stampa un elenco di comandi disponibili;\n");
	printf("- plist, stampa un elenco di processi generati dalla shell costum;\n");
	printf("- pnew <nome>, crea un nuovo processo chiamato <nome>;\n");
	printf("- pinfo <nome>, stampa le informazioni relative al processo chiamato <nome>;\n");
	printf("- pclose <nome>, chiede al processo chiamato <nome> di chiudersi;\n");
	printf("- quit, esce dalla shell costum e chiude tutti i processi attivi;\n");
	printf("COMANDI AVANZATI:\n");
	printf("- pspawn <nome>, chiede al processo chiamato <nome> di clonarsi, creando un altro processo chiamato <nome_i> con i progressivo;\n");
	printf("- prmall <nome>, chiede al processo chiamato <nome> di chiudersi, chiudendo eventuali cloni;\n");
	printf("- ptree, mostra la gerarchia completa dei processi gerati ancora attivi;\n");
	printf("\n");
	printf(reset);
}



