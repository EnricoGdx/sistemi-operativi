#ifndef ALBERO
#define ALBERO

struct processo{
	
	int pid;
	int ppid;
	int figli;
	char nome[256];
	struct processo* padre;
	struct processo* figlio;
	struct processo* fratello;	
};

struct processo* inizializza();
void inserisciFiglio(struct processo* padre, struct processo* figlio);
void eliminaFiglio(char* nome, struct processo* radice);
struct processo* cerca(char* nome, struct processo* p, int padre);
void stampaSpazi();
void stampaAlbero(struct processo* albero);
void stampaLivelli(struct processo* albero);
void stampaLista(struct processo* p);

#endif
