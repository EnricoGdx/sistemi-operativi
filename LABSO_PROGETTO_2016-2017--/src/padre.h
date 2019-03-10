#ifndef PADRE
#define PADRE

#include "albero.h"

void pnew(char* nome);
void plist();
void pinfo(char* nome);
void prclose(char* nome, int i);
void quit(char* nome);
void pspawn(char* nome);
void prmall(char* nome, char* partenza, int i);
void ptree();
void pchname(char* vecchio, char* nuovo, int i);
void pchnamesupporto(char* nuovo, char* vecchio, struct processo* p, int i);
void ptreetab();
void ptable();
int trovaCarattere(char* stringa, char carattere);
struct processo* prendiRadice();

#endif
