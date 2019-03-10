.PHONY: build assets test
help:
	@echo Componenti del gruppo:;
	@echo - Enrico
	@echo - Riccardo
	@echo - Valentina Odorizzi 180353;
	@echo
	@echo Descrizione del progetto:;
	@echo
	@echo Regole utilizzabili:
	@echo - help, regola che permette di visualizzare i componenti del gruppo, informazioni generali relative al progetto e le regole utilizzabili;
	@echo - clean, regola che cancella le cartelle build e assest se presenti; 
	@echo - build, regola che crea una cartella chiamata "build" contenente i file eseguibili;
	@echo - assest, regola che crea una cartella chiamata "assest" contenente i file di supporto;
	@echo - test, regola che esegui il progetto in modalità "test".

clean:
	@rm -rf -f build;
	@rm -rf -f assest;

build:
	@make clean;
	@mkdir build;
	@gcc ./src/pmanager.c -o ./build/pmanager;

assets:
	@make build;
	@mkdir assets;

test:
	@make assets;
