//#include<stdio.h>
#include<iostream>
#include<string>

using namespace std;

void main()
{
	int iCaseNum,i,j;
	string strInt1,strInt2,strSum;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>strInt1>>strInt2;
		strSum="";
		strInt1.erase(0,strInt1.find_first_not_of('0'));
		strInt1.erase(strInt1.find_last_not_of('0')+1,INT_MAX);
		strInt2.erase(0,strInt2.find_first_not_of('0'));
		strInt2.erase(strInt2.find_last_not_of('0')+1,INT_MAX);
		for(j=0;j<strInt1.length() || j<strInt2.length();j++)
		{
			if(strSum.length()<=j)
				strSum.append("0");
			if(j<strInt1.length() && j<strInt2.length())
				strSum[j]+=strInt1[j]+strInt2[j]-2*'0';	
			else if(strInt1.length()>strInt2.length())
				strSum[j]+=strInt1[j]-'0';
			else
				strSum[j]+=strInt2[j]-'0';
			if(strSum[j]>'9')
			{
				strSum[j]-=10;
				strSum.append("1");
			}
		}
		strSum.erase(0,strSum.find_first_not_of('0'));
		cout<<strSum<<endl;
	}
}