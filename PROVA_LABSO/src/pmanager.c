#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void test();
void comandi();
void help();
void list();
void new(char* nome);
void info(char* nome);
void myclose(char* nome);
void quit();
void spawn(char* nome);
void rmall(char* nome);
void figlio();
void pinfohandler(int sig);
void pclosehandler(int sig);

int pid,npid=0;
int pidlist [100];
char nomep[50];

void pclosehandler(int sig){

	printf("close");
}

void pinfohandler(int sig){

	printf("info");

}


int main(int argc, char** argv)
{
	if(argc == 1)
	{
		comandi();
	}
	else if(argc == 2)
	{
		test();
	}

	return(0);
}

void test()
{

}

void comandi()
{
	char istruzione[100],nome[50];
	char* ist;
	do{
		printf("Inserisci un comando disponibile:\n");
		scanf("%s", istruzione);
	
		ist = strtok(istruzione, " ");
		
		/*switch(ist)
		{
			case strcmp(ist, "phelp") == 0: ; break;
			case strcmp(ist, "plist") == 0: ; break;
			case strcmp(ist, "pnew") == 0: ; break;
			case strcmp(ist, "pinfo") == 0: ; break;
			case strcmp(ist, "pclose") == 0: ; break;
			case strcmp(ist, "quit") == 0: ; break;
			case strcmp(ist, "pspawn") == 0: ; break;
			case strcmp(ist, "prmall") == 0: ; break;
			case strcmp(ist, "ptree") == 0: ; break;
		}*/

		if(strcmp(ist, "phelp") == 0)
		{
			help();
		}else if(strcmp(ist, "plist") == 0)
		{
			list();
		}else if(strcmp(ist, "pnew") == 0)
		{
			printf("Inserisci un nome:\n");
			scanf("%s", nome);
			new(nome);
		}else if(strcmp(ist, "pinfo") == 0)
		{
			printf("Inserisci un nome:\n");
			scanf("%s", nome);
			info(nome);
		}else if(strcmp(ist, "pclose") == 0)
		{
		
		}else if(strcmp(ist, "quit") == 0)
		{
			
		}else if(strcmp(ist, "pspawn") == 0)
		{

		}else if(strcmp(ist, "prmall") == 0)
		{
	
		}else if(strcmp(ist, "ptree") == 0)
		{
			
		}

	}while(strcmp(istruzione, "quit") != 0);
}

void help()
{
	printf("\n");
	printf("I comandi disponibili sono:\n");
	printf("COMANDI BASE:\n");
	printf("- phelp, stampa un elenco di comandi disponibili;\n");
	printf("- plist, stampa un elenco di processi generati dalla shell costum;\n");
	printf("- pnew <nome>, crea un nuovo processo chiamato <nome>;\n");
	printf("- pinfo <nome>, stampa le informazioni relative al processo chiamato <nome>;\n");
	printf("- pclose <nome>, chiede al processo chiamato <nome> di chiudersi;\n");
	printf("- quit, esce dalla shell costum;\n");
	printf("COMANDI AVANZATI:\n");
	printf("- pspawn <nome>, chiede al processo chiamato <nome> di clonarsi, creando un altro processo chiamato <nome_i> con i progressivo;\n");
	printf("- prmall <nome>, chiede al processo chiamato <nome> di chiudersi, chiudendo eventuali cloni;\n");
	printf("- ptree, mostra la gerarchia completa dei processi gerati ancora attivi;\n");
	printf("\n");
}

void list()
{
	system("ps");
}

void new(char* nome)
{
	//check no process
	pid = fork();
	if(pid < 0) printf("Errore nella creazione del processo");
	else if(pid == 0){
	    printf("Creando il figlio\n");
	    strcpy(nomep,nome);
	    figlio();
	}
	pidlist[npid]=pid;
	npid++;	

}

void info(char* nome)
{
	int i;
	for(i=0; i<npid; i++){
		kill(pidlist[i],SIGUSR1);
	}
	
}

void myclose(char* nome)
{

}

void quit()
{

}

void spawn(char* nome)
{

}

void rmall(char* nome)
{

}

void figlio()
{
	int i = 1;
	printf("creato");
	while(i > 0){
		if(signal(SIGUSR1, pinfohandler) == SIG_ERR || signal(SIGUSR2, pclosehandler) == SIG_ERR) 
		   printf("Errore nell'installazione dell'handler");
		else{
			i++;
        		sleep(7);
		}
	}
}

