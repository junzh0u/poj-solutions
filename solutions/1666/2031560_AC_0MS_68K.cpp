#include<iostream.h>
#include<string.h>

#define MAX 1000

bool Same(int* iArray,const int iSize)
{
	int i;
	for(i=1;i<iSize;i++)
		if(iArray[i]!=iArray[i-1])
			return false;
	return true;
}

void main()
{
	int iStuNum,iCandy[MAX],iTemp[MAX],i,iCount;
	while(cin>>iStuNum)
	{
		if(iStuNum==0)
			break;
		for(i=0;i<iStuNum;i++)
			cin>>iCandy[i];
		/*for(i=0;i<iStuNum;i++)
			cout<<iCandy[i]<<" ";
		cout<<endl;*/
		iCount=0;
		while(!Same(iCandy,iStuNum))
		{
			iTemp[0]=iCandy[0]/2+iCandy[iStuNum-1]/2;
			for(i=1;i<iStuNum;i++)
				iTemp[i]=iCandy[i]/2+iCandy[i-1]/2;
			for(i=0;i<iStuNum;i++)
				iTemp[i]+=iTemp[i]%2;
			iCount++;
			memcpy(iCandy,iTemp,sizeof(int)*iStuNum);
			/*for(i=0;i<iStuNum;i++)
				cout<<iCandy[i]<<" ";
			cout<<endl;*/
		}
		cout<<iCount<<" "<<iCandy[0]<<endl;
	}
}