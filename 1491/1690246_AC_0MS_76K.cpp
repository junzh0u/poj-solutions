#include<iostream.h>
#include<iomanip.h>
#include<math.h>

bool PRIME(int,int);

void main()
{
	unsigned long int iDataNum,iData[50],i,j,iCount;
	while(1)
	{
		cin>>iDataNum;
		if(!iDataNum)
			break;
		for(i=0;i<iDataNum;i++)
			cin>>iData[i];
		iCount=0;
		for(i=0;i<iDataNum-1;i++)
			for(j=i+1;j<iDataNum;j++)
				if(PRIME(iData[i],iData[j]))
					iCount++;
		if(!iCount)
			cout<<"No estimate for this data set."<<endl;
		else
			cout<<setiosflags(ios::fixed)<<setprecision(6)<<float(sqrt(3.0*iDataNum*(iDataNum-1)/iCount))<<endl;
	}
}

bool PRIME(int iA,int iB)
{
	if(iA==1 || iB==1)
		return true;
	//if(iA==iB)
		//return false;
	if(iA>iB)
	{
		if(!(iA%iB))
			return false;
		return PRIME(iB,iA%iB);
	}
	if(iB>iA)
	{
		if(!(iB%iA))
			return false;
		return PRIME(iA,iB%iA);
	}
}