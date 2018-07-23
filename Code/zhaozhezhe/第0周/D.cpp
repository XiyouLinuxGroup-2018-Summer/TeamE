#include <iostream>
using namespace std;
int main(void){

	int a,b=0,n,i=0;
	while(1){
		cin >> n;
		if(n == 0){
			break;
		}
		else{
			while(i++<n){
				cin >> a;
				b+=a;
			}
			cout << b << endl;
			n = 0;
			i = 0;
			b=0;
		}
	}
       	return 0;
}
