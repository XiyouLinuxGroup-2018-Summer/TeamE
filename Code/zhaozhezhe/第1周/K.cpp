#include <iostream>

using namespace std;
int OK(int n){
	int i;
	for(i=2; i<n; i++){
		if(n%i==0)
			break;	
	}
	if(i == n){
		return 1;
	}else{
		return 0;
	}
}
int main(void){

	int x,y;
	
	while(cin >> x >> y){
		if(x == 0 && y == 0){
			break;
		}
		int flag = 0;
		for(int i=x; i<=y; i++){
			int j = i*i+i+41;
			if(OK(j)){
				flag += 0;
			}else{
				flag += 1;
			}			
		}
		if(!flag){
			cout << "OK" << endl;
		}else{
			cout << "Sorry" << endl;
		}
	}
	return 0;
}
