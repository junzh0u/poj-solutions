#include<iostream>
using namespace std;

int main(){
	int height;
	while(!cin.eof()){
		cin>>height;
		if(height<168){
			cout<<"CRASH "<<height<<endl;
			return 0;
		}
	}
	cout<<"NO CRASH"<<endl;
}