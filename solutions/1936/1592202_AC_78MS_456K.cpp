#include<iostream>
#include<string>

using namespace std;

void main()
{
	string strS,strT;
	int i,j,iBreakValue;
	while(cin>>strS>>strT)
	{
		j=0;
		iBreakValue=0;
		for(i=0;i<strS.length() && iBreakValue==0;i++)
		{
			iBreakValue=1;
			for(j;j<strT.length();j++)
				if(strS[i]==strT[j])
				{
					iBreakValue=0;
					j++;
					break;
				}
		}
		if(iBreakValue)
			cout<<"No"<<endl;
		else
			cout<<"Yes"<<endl;
	}
}