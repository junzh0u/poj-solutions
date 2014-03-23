#include<iostream.h>

void main()
{
	int iGuestNum,iGuest[30],iSum,iSam,i;
	while(1)
	{
		cin>>iGuestNum;
		if(!iGuestNum)
			break;
		iSum=0;
		for(i=0;i<iGuestNum;i++)
		{
			cin>>iGuest[i];
			iSum+=iGuest[i];
		}
		if(iSum%2)
			cout<<"No equal partitioning."<<endl;
		else
		{
			iSam=0;
			for(i=0;i<iGuestNum;i++)
			{
				iSam+=iGuest[i];
				if(iSam==iSum/2)
				{
					cout<<"Sam stops at position "<<i+1<<" and Ella stops at position "<<i+2<<"."<<endl;
					break;
				}
				else if(iSam>iSum/2)
				{
					cout<<"No equal partitioning."<<endl;
					break;
				}
			}
		}
	}
}