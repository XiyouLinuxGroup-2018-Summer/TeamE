#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

void Print(int const &n){
	cout << n;
}
int main(void){
	
	int n, m;
	while(cin >> n >> m){
		if(n ==0 && m ==0){
			break;
		}

		vector<int> myvector;
		while(n--){
			int k;
			cin >> k;
			myvector.push_back(k);
		}
		myvector.push_back(m);
		sort(myvector.begin(),myvector.end());

		//for_each(myvector.begin(),myvector.end(),Print);
		//
		//
#if 0
		while(myvector.begin() != myvector.end() ||  !myvector.empty()){
			int l =  myvector.back();
			cout << l << " ";
			myvector.pop_back();
		}
		int l = myvector.back();
		cout << l <<endl;
		myvector.pop_back();

		myvector.clear();
#endif 		
		for (int i=0; i<myvector.size()-1; i++)
			cout << myvector.at(i) << " ";
		cout << myvector.at(myvector.size()-1)<<endl;
	}

	return 0;
}
