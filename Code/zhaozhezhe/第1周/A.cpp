#include <iostream>
#include <stdio.h>
using namespace std;
void chage(char &a, char &b){
	char c = a;
	a = b;
	b = c;
}
void swap(char &a, char &b, char &c){
	if(a>b){
		chage(a,b);
		if(a>c){
			chage(a,c);
		}
		if(b>c){
			chage(b,c);
		}
	}else{
		if(a>c){
			chage(a,c);
		}
		if(b>c){
			chage(b,c);
		}
	}
}
int main(void){

	char a,b,c;
	while(cin >>a>>b>>c){
		swap(a,b,c);
		printf("%c %c %c\n",a,b,c);
	}
	return 0;
}
