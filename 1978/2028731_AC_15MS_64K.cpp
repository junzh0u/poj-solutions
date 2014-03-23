#include<iostream.h>
#include<string.h>

void main()
{
	int iCard[50],iCardNum,iCutNum,iBefore,iCut,i,iTemp[50];
	while(cin>>iCardNum>>iCutNum)
	{
		if(!iCardNum)
			break;
		for(i=0;i<iCardNum;i++)
			iCard[i]=iCardNum-i;
		while(iCutNum--)
		{
			cin>>iBefore>>iCut;
			memcpy(iTemp,iCard,(iBefore-1)*sizeof(int));
			memcpy(iCard,iCard+iBefore-1,iCut*sizeof(int));
			memcpy(iCard+iCut,iTemp,(iBefore-1)*sizeof(int));
		}
		cout<<iCard[0]<<endl;
	}
}