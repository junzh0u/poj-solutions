#include<iostream.h>

void main()
{
	char cString[258];
	int i,iSum;
	while(1)
	{
		cin.getline(cString,257);
		if(cString[0]=='#')
			break;
		iSum=0;
		for(i=0;cString[i]!='\0';i++)
			if(cString[i]!=' ')
				iSum+=(cString[i]-'A'+1)*(i+1);
		cout<<iSum<<endl;
	}
}