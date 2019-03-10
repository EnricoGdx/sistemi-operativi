#ifndef ALBERO
#define ALBERO

struct processo{
	
	int vivo;
	int visitato;
	int pid;
	int ppid;
	int cloni;
	int figli;
	char nome[256];
	struct processo* padre;
	struct processo* figlio;
	struct processo* fratello;	
};

struct processo* inizializza();
void inserisciFiglio(struct processo* padre, struct processo* figlio);
void eliminaFiglio(char* nome, struct processo* radice);
struct processo* cerca(char* nome, struct processo* p);
void stampaSpazi();
void stampaAlbero(struct processo* albero);
void stampaLivelli(struct processo* albero);
void stampaLista(struct processo* p);
void stampaListaDettagliata(struct processo* p);
void elimina(struct processo* p);
//int ricerca(int contatore, struct processo* p);

#endif
