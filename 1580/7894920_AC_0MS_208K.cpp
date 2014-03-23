#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<string>
using namespace std;

int gcd(int l,int g){
	if(g%l==0)
		return l;
	return gcd(g%l,l);
}

int main(){
	string a,b;
	while(cin>>a>>b){
		int appxMax=0;
		for(int shift=b.length()-1;shift+a.length()>=1;shift--){
			int appx=0;
			for(int i=0;i<a.length();i++)
				if(i+shift>=0 && i+shift<b.length() && a[i]==b[i+shift])
					appx++;
			if(appxMax<appx)
				appxMax=appx;
		}
		cout<<"appx("<<a<<","<<b<<") = ";
		appxMax*=2;
		int lenSum=a.length()+b.length();
		if(appxMax==0)
			cout<<"0";
		else if(appxMax==lenSum)
			cout<<"1";
		else{
			int divisor=gcd(appxMax,lenSum);
			cout<<appxMax/divisor<<"/"<<lenSum/divisor;
		}
		cout<<endl;
	}
	return 0;
}