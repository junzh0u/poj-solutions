#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<sstream>
#include<string>
using namespace std;

const int DEGREE_MAX=8;

int main(){
	while(true){
		int coefs[DEGREE_MAX+1];
		for(int i=0;i<=DEGREE_MAX;i++)
			cin>>coefs[i];
		if(!cin)
			return 0;
		ostringstream format;
		bool zero=true,first=true;
		for(int i=0;i<=DEGREE_MAX;i++){
			if(coefs[i]==0)
				continue;
			if(first){
				if(coefs[i]<0)
					format<<"-";
				first=false;
			}
			else{
				if(coefs[i]<0)
					format<<" - ";
				else
					format<<" + ";
			}
			int degree=DEGREE_MAX-i;
			if(abs(coefs[i])>1 || degree==0)
				format<<abs(coefs[i]);
			if(degree>0)
				format<<"x";
			if(degree>1)
				format<<"^"<<degree;
			zero=false;
		}
		if(zero)
			format<<"0";
		format.flush();
		cout<<format.str()<<endl;
	}
}