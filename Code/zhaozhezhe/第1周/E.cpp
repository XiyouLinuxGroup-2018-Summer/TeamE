#include <iostream>
#include <stdio.h>

using namespace std;

int main(void){
	char C;
	int A,B;
	int n;
	cin >> n;
	while(n--){
	cin >> C;
	cin >> A >> B;

	if(C == '+'){
		cout << A+B << endl;
	}
	if(C == '-'){
		cout << A-B << endl;
	}
	if(C == '*'){
		cout << A*B << endl;
	}
	if(C == '/'){
		if(A%B == 0){
			cout << A/B << endl;
		}else{
			printf("%.2f\n", A/(B*1.0));
		}
	}
	}
}
