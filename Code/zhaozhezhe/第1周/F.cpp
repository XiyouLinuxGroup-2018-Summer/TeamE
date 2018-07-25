#include <iostream>
#include <string>
using namespace std;

int main(void){
	string test;
	int n;
	cin >> n;
	while(n--){
		cin >> test;
		int sum = 0;
		for (int i=0; i<test.length(); i++){
			if(test[i]>='0' && test[i]<='9'){
				sum++;
			}
		}
		cout << sum << endl;
		test.clear();
	}
}
