#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

int Pow2Mod10(int exp){
	int modulus[15]={2};
	for(int i=1;i<=ceil(log(double(exp))/log(2.0));i++)
		modulus[i]=(modulus[i-1]*modulus[i-1])%10;
	int result=1;
	for(i=ceil(log(double(exp))/log(2.0));i>=0;i--)
		if(exp>=pow(2.0,i)){
			result=(result*modulus[i])%10;
			exp-=pow(2.0,i);
		}
	return result;
}

int LastNonzeroDigit(int num){
	if(num==0)
		return 0;
	if(num%10==0)
		return LastNonzeroDigit(num/10);
	return num%10;
}

int FactorialLastNonzeroDigit(int exp){
	if(exp==0)
		return 1;
	int digit=LastNonzeroDigit(FactorialLastNonzeroDigit(exp/5)*Pow2Mod10(exp/5));
	for(int i=0;i<exp%5;i++)
		digit=LastNonzeroDigit(digit*(exp-i));
	return digit;
}

int main(){
	int num;
	while(cin>>num)
		cout<<setw(5)<<num<<" -> "<<FactorialLastNonzeroDigit(num)<<endl;
	return 0;
}