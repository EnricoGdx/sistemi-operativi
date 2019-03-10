#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXRAND 40
#define AS 15

int main() {
	FILE* stream;
	stream = fopen("./assets/testo.txt", "w+");
	
	if(stream == NULL) {
		printf("Impossibile creare il file di test\n");
		exit(1);
	}
	
	int comando, i, n, s, e, r;								//contatori e temporanei vari
	int processi[AS] = {0};									//array per i processi aperti
	srand(time(NULL));										//seed del random per un output diverso ad ogni esecuzione
				
	n = rand() % MAXRAND;									//numero di iterazioni scelte a caso, il massimo è definito dalla costante
	
	fprintf(stream,"phelp\n");								//la prima istruzione scritta è sempre il comando phelp
	
	for (i=0;i<AS;i++) {
		fprintf(stream,"pnew processo%d\n",i);				//crea i processi con nome processo0, processo1...
		processi[i]=1;
	}
	r=0;
	for (i=0;i<n;i++) {
		do {
			s = rand()%11;
			e = rand()%AS;
		} while ((s==4 || s==6 || s==7 || s==8 || s==9 || s==10) && processi[e]!=1);		//scrivere la condizione in questo modo per evitare endless loop
		switch (s) {
			case 1:
				fprintf(stream,"ptree\n");
				break;
			case 2:
				fprintf(stream,"plist\n");
				break;
			case 3:
				fprintf(stream,"ptable\n",e);
				break;
			case 4:
				fprintf(stream,"pinfo processo%d\n",e);
				break;
			case 5:
				fprintf(stream,"ptreetab\n",e);
				break;
			case 6:
				if (r==0) {
					fprintf(stream,"pchname processo%d nuovoprocesso\n",e);
					r=1;
					processi[e]=0;
				} else {
					fprintf(stream,"pinfo nuovoprocesso\n");
				}
				break;
			case 7:
			case 8:
				fprintf(stream,"pspawn processo%d\n",e);
				break;
			case 9:
				fprintf(stream,"pclose processo%d\n",e);
				processi[e]=0;
				break;
			case 10:
				fprintf(stream,"prmall processo%d\n",e);
				processi[e]=0;
				break;
			default:
				break;
		}
	}
	fprintf(stream,"ptreetab\n");								//stampa un albero prima di chiudere la shell custom
	fprintf(stream,"quit\n");									//l'ultima istruzione scritta è sempre il comando quit
return 0;
}
