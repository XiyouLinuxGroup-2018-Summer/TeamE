#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int main(void){

	int a[5][5];
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			//int b;
			cin >> a[i][j];
			//a[i][j] = b;
		}
	}
	vector<int> x;
	vector<int> y;


	x.push_back(0);
	y.push_back(0);

	for(int i=0,j=0; i!=4 || j != 4;){
		if(i == 4 && j == 4){
			break;
		}
		if(a[i+1][j] == 0){
			x.push_back(i+1);
			y.push_back(j);
			i = i+1;
			j = j;
		}else if(a[i][j+1] == 0){
			x.push_back(i);
			y.push_back(j+1);
			i = i;
			j = j+1;
		}else{
			a[x.back()][y.back()] = 1;
			x.pop_back();
			y.pop_back();	
			i = x.back();
			j = y.back();
		}
	}
	vector<int>::iterator itx = x.begin();
	vector<int>::iterator ity = y.begin();

	while(itx != x.end() && ity != y.end()){
		cout << "(" << *itx << "," << *ity << ")" << endl;
		itx++;
		ity++;
	}
	return 0;
}
