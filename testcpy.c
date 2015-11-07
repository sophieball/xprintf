#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char* p;
	char* q;
	int i = 0;
	p = (char*) malloc(10);
	q = p;
	for(i; i<9; i++){
		*p = 'i';
		p++;
	}
	*p = '\0';
	printf("%s\n",q);
}