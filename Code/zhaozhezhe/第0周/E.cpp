#include <iostream>
using namespace std;
int main(void){

	int a,b=0,n1,n2,i=0,j=0;
	cin >> n1;
	while(j++<n1){
			cin >> n2;
			while(i++<n2){
				cin >> a;
				b+=a;
			}
			cout << b << endl;
			i = 0;
			b = 0;
	}
       	return 0;
}
