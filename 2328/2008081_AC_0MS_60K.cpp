#include<iostream.h>
#include<string.h>

#define LOW 1
#define ON 2
#define HIGH 3

void main()
{
	int iTable[10]={0},iIn,i;
	char cDenote[10];
	bool bHonest=true;
	while(cin>>iIn)
	{
		if(!iIn)
			break;
		cin.ignore();cin.getline(cDenote,10);
		if(!strcmp(cDenote,"too low"))
		{
			if(iTable[iIn-1]==HIGH)
				bHonest=false;
			else
				iTable[iIn-1]=LOW;
		}
		else if(!strcmp(cDenote,"too high"))
		{
			if(iTable[iIn-1]==LOW)
				bHonest=false;
			else
				iTable[iIn-1]=HIGH;
		}
		else
		{
			if(iTable[iIn-1]!=0)
				bHonest=false;
			for(i=0;i<iIn-1;i++)
				if(iTable[i]==HIGH)
					bHonest=false;
			for(i=iIn;i<10;i++)
				if(iTable[i]==LOW)
					bHonest=false;
				cout<<(bHonest?"Stan may be honest":"Stan is dishonest")<<endl;
			memset(iTable,0,sizeof(iTable));
			bHonest=true;
		}
	}
}