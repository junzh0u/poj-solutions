#include<iostream.h>
#include<string.h>

char cMachine[90];

double RollLeft(int);
double RollRight(int);

void main()
{
	int i;
	double dProb;
	while(1)
	{
		cin.getline(cMachine,89);
		if(cMachine[0]=='#')
			break;
		dProb=0;
		for(i=0;cMachine[i]!='\0';i++)
		{
			if(cMachine[i]=='.')
				dProb+=100;
			else if(cMachine[i]=='/')
				dProb+=RollLeft(i);
			else if(cMachine[i]=='\\')
				dProb+=RollRight(i);
			else if(cMachine[i]=='|')
				dProb+=0.5*(RollLeft(i)+RollRight(i));
		}
		cout<<(int)(dProb/i)<<endl;
	}
}

double RollLeft(int iPos)
{
	int i;
	bool bFell=true;
	for(i=iPos-1;i>=0;i--)
	{
		if(cMachine[i]=='.')
			break;
		else if(cMachine[i]=='|' || cMachine[i]=='\\')
		{
			bFell=false;
			break;
		}
	}
	if(bFell)
		return 100;
	else
		return 0;
}

double RollRight(int iPos)
{
	int i;
	bool bFell=true;
	for(i=iPos+1;i<strlen(cMachine);i++)
	{
		if(cMachine[i]=='.')
			break;
		else if(cMachine[i]=='|' || cMachine[i]=='/')
		{
			bFell=false;
			break;
		}
	}
	if(bFell)
		return 100;
	else
		return 0;
}