.PHONY: build assets test
help:
	@echo Componenti del gruppo:;
	@echo - Enrico Egidi 180884;
	@echo - Riccardo Visonà 179480;
	@echo - Valentina Odorizzi 180353;
	@echo
	@echo Descrizione del progetto:Consente all\' utente di creare o clonare processi, con nomi da specificare, avendo come riferimento 		      una struttura dati ad essi associata e consentendo anche la loro terminazione, la ricerca di informazioni riguardanti uno  		      specifico processo o\/e della gerarchia formatasi. Un vincolo nella creazione o clonazione di processi è quello di non poter  		      avere processi con lo stesso  nome nemmeno se  uno degli stessi fosse stato precedentemente terminato.
	@echo Per eseguire il programma dopo aver creato la directory assets o build, è possibile utilizzare
	@echo l\'eseguibile stesso che si trova nella build e che si chiama pmanager, utilizzare lo shortcut fornito,
	@echo ovvero makerun, o se si dispone di un testo con scritte istruzioni inerenti alla natura del programma, tramite maketest.
	@echo Le funzioni che il programma supporta sono descritte e reperibili tramite phelp
	@echo
	@echo Regole utilizzabili:
	@echo - help, regola che permette di visualizzare i componenti del gruppo, informazioni generali relative al progetto e le regole utilizzabili;
	@echo - clean, regola che cancella le cartelle build e assets se presenti; 
	@echo - build, regola che crea una cartella chiamata "build" contenente i file eseguibili;
	@echo - assets, regola che crea una cartella chiamata "assets" contenente i file di supporto;
	@echo - test, regola che esegue il progetto in modalità "test" NOTA: richiede un file di "test" nella cartella "assets";
	@echo - test_dyn, regola che crea il file di test dinamicamente ed esegue il progetto in modalità "test";
	@echo - run, regola che esegue il file eseguibile pmanager;
clean:
	@rm -rf build;
	@rm -rf assets;
	@rm -rf tmp;

build:
	@make clean;
	@mkdir build;
	@mkdir tmp;
	@gcc ./src/pmanager.c ./src/albero.c ./src/figlio.c ./src/padre.c -o ./build/pmanager;

assets:
	@make build;
	@mkdir assets;

test:
	@make assets;
	@./build/pmanager ./assets/testo.txt;

test_dyn:
	@make assets;
	@gcc ./src/test.c -o ./build/test;
	@./build/test;
	@./build/pmanager ./assets/testo.txt;

run:
	@make build;
	@./build/pmanager;
	
