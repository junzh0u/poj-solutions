#include<iostream.h>

void main()
{
	char cIni[12],cStep[12];
	int i;
	cin>>cIni;
	while(cin>>cStep)
		for(i=0;cStep[i]!='\0';i++)
			cIni[i]=(cIni[i]-'0'+cStep[i]-'0')%10+'0';
	cout<<cIni<<endl;
}
