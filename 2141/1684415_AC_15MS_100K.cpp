#include<iostream>
#include<cctype>

using namespace std;

void main()
{
	char cKey[30],cSecMes[90];
	int i;
	cin.getline(cKey,29,'\n');
	cin.getline(cSecMes,89,'\n');
	for(i=0;cSecMes[i]!='\0';i++)
	{
		if(islower(cSecMes[i]))
			cSecMes[i]=cKey[cSecMes[i]-'a'];
		else if(isupper(cSecMes[i]))
			cSecMes[i]=toupper(cKey[cSecMes[i]-'A']);
	}
	cout<<cSecMes<<endl;
}
