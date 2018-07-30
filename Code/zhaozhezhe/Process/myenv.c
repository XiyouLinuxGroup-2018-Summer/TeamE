#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char **argv) {

	int i;

	printf("Argument : \n");
	for (i=0; i<argc; i++) {
		printf("argv[%d] is %s\n", i, argv[i]);
	}

	printf("Enviroment: \n");

	for (i=0; environ[i]!=NULL; i++) {
		printf("%s\n", environ[i]);
	}

	return 0;
}
