#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	FILE* stream;
	stream = fopen("assets/testo.txt", w+);

	if(stream == NULL)
	{
		printf("Impossibile creare il file di test\n");
		exit(1);
	}
	
}

void comandi(FILE* stream)
{
	int comando;
	srand(time(NULL));

	
	
}


