#include<iostream.h>

void main()
{
	int iStackNum,iHeight[50],i,iSum,iMove,iCount=0;
	while(1)
	{
		cin>>iStackNum;
		if(!iStackNum)
			break;
		iCount++;
		iSum=0;
		for(i=0;i<iStackNum;i++)
		{
			cin>>iHeight[i];
			iSum+=iHeight[i];
		}
		iMove=0;
		for(i=0;i<iStackNum;i++)
			if(iHeight[i]>iSum/iStackNum)
				iMove+=iHeight[i]-iSum/iStackNum;
		cout<<"Set #"<<iCount<<"\nThe minimum number of moves is "<<iMove<<".\n"<<endl;
	}
}
