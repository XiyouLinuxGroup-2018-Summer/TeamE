#include <stdio.h>
#include <string.h>
int main(void){

	char s[100];
	char s1[100];
	int len, i, j = 0, k;
	char d;
	int n;

	scanf("%d",&n);
	while(j++<n){
		scanf("%s", s);
		strcpy(s1,s);
		len = strlen(s);
		for(i=0; i<len/2; i++){
			d = s1[i];
			s1[i] = s1[len-1-i]; 
			s1[len-1-i] = d;
		}
		if(strcmp(s1,s) == 0){
			printf("yes");
		}else{
			printf("no");
		}
		if(j <= n){
			printf("\n");
		}
		
	}
}
