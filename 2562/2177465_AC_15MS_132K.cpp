#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

void main()
{
	string strLeft,strRight;int i,iCarry;
	while(cin>>strLeft>>strRight && (strLeft.compare("0")!=0 || strRight.compare("0")!=0))
	{
		reverse(strLeft.begin(),strLeft.end());
		reverse(strRight.begin(),strRight.end());
		for(i=strLeft.length();i<=11;i++)strLeft+='0';
		for(i=strRight.length();i<=11;i++)strRight+='0';
		for(i=0,iCarry=0;i<10;i++)
			if(strLeft[i]+strRight[i]>=2*'0'+10)
			{
				iCarry++;
				strLeft[i+1]++;
			}
		if(iCarry==0)cout<<"No carry operation."<<endl;
		else if(iCarry==1)cout<<"1 carry operation."<<endl;
		else cout<<iCarry<<" carry operations."<<endl;
	}
}
