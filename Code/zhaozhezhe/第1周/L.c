#include <stdio.h>
#include <string.h>


int main(void){
	
	char s[50];
	while(gets(s)){
		int len = strlen(s);
		for(int i=0; i<len; i++){
			if(i==0){
				s[i] = s[i] - 32;
			}
			if(s[i] == ' '){
				s[i+1] = s[i+1] - 32;
			
			}

			printf("%c",s[i]);
		}
		printf("\n");
	}
	return 0;
}
