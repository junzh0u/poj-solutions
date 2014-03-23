#include<iostream>
using namespace std;

int main(){
	int degree,coefficient[21];
	cin>>degree;
	for(int i=degree;i>=0;i--)
		cin>>coefficient[i];
	if(degree>2 || (degree==2 && coefficient[1]*coefficient[1]-4*coefficient[0]*coefficient[2]>=0))
		cout<<"NO"<<endl;
	else
		cout<<"YES"<<endl;
	return 0;
}