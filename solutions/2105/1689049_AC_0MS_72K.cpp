#include<iostream.h>
#include<math.h>

void main()
{
	int iCaseNum,i,j,iTemp;
	char cAddress[40];
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>cAddress;
		for(j=0;j<32;j++)
		{
			if(!(j%8))
			{
				if(j)
					cout<<iTemp<<".";
				iTemp=0;
			}
			iTemp+=(cAddress[j]-'0')*pow(2,7-j%8);
		}
		cout<<iTemp<<endl;
	}
}