#include<iostream>
#include<cmath>
using namespace std;

int main(){
	double exponent,power;
	while(cin>>exponent>>power)
		cout<<int(pow(power,1.0/exponent)+0.1)<<endl;
}