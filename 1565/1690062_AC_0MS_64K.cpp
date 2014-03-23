//#include<iostream>
#include<string>
#include<cmath>
#include<cstdio>

using namespace std;

void main()
{
	//string strArray;
	char cArray[40];
	__int64 dDec;
	int i;
	while(1)
	{
		//cin>>strArray;
		//cin>>cArray;
		scanf("%s",&cArray);
		if(cArray[0]=='0')
			break;
		dDec=0;
		for(i=0;i<strlen(cArray);i++)
		{
			dDec+=(cArray[i]-'0')*(pow(2,strlen(cArray)-i)-1);
			//cout<<"dDec+="<<(strArray[i]-'0')*(pow(2,strArray.length()-i)-1)<<endl;
			if(cArray[i]=='2')
				break;
		}
		printf("%I64d\n",dDec);
	}
}