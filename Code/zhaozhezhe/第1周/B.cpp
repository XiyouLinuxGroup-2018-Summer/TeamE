#include <iostream>
#include <iomanip>
#define PI 3.1415927

using namespace std;

int main(void){

	double a;
	while(cin >> a){
		double b = (4/3.)*PI*a*a*a;
		cout << fixed << setprecision(3) <<b<< endl;
	}
	return 0;
}
