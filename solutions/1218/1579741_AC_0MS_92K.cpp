#include<iostream.h>

void main()
{
	int iCaseNum,iN,iEscNum,i,j,k;
	bool *bArray;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iN;
		bArray=new bool [iN];
		iEscNum=0;
		for(j=0;j<iN;j++)
			bArray[j]=true;
		for(j=2;j<=iN;j++)
			for(k=1;k*j<=iN;k++)
				bArray[k*j-1]=bArray[k*j-1]?false:true;	
		for(j=0;j<iN;j++)
			if(bArray[j])
				iEscNum++;
		cout<<iEscNum<<endl;
	}
}