#include<iostream.h>

const int OPP[6]={5,3,4,1,2,0};

int Max(int* iDice,int iBottom)
{
	int i,j;
	for(i=0;i<6;i++)
		if(iDice[i]==iBottom)
			break;
	for(j=6;j>=1;j--)
		if(j!=iDice[i] && j!=iDice[OPP[i]])
			return j;
}

void main()
{
	int iCaseNum,iDiceNum,iDice[10000][6],i,j,k,iBottom;
	long iMax,iSum;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iDiceNum;
		for(i=0;i<iDiceNum;i++)
			for(j=0;j<6;j++)
				cin>>iDice[i][j];
		iMax=0;
		for(i=0;i<6;i++)
		{
			iSum=0;
			iBottom=iDice[0][i];
			for(j=0;j<iDiceNum;j++)
			{
				iSum+=Max(iDice[j],iBottom);
				for(k=0;k<6;k++)
					if(iDice[j][k]==iBottom)
					{
						iBottom=iDice[j][OPP[k]];
						break;
					}
			}
			if(iSum>iMax)
				iMax=iSum;
		}
		cout<<iMax<<endl;
	}
}
