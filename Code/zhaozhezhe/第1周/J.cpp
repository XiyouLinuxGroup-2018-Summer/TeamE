#include <iostream>

using namespace std;

int main(void){
	int n;
	int a, b;
	cin >> n;
	while(n--){
		cin >> a >> b;
		int x=0, y=0;
		for(int i=1; i<=(a/2); i++){
			if(a%i == 0){
				x+=i;
			}
		}
		if(x == b){
			for(int i=1; i<=(b/2); i++){
				if(b%i == 0){
					y+=i;
				}
			}
			if(y == a){
				cout << "YES" << endl;
			}else{
				cout << "NO" << endl;
			}
		}else{
			cout << "NO" << endl;
		}
	}
}
