#include<iostream.h>

void main()
{
	int iUgly[1500]={1,2,3,4,5,6,8,9,10,12};
	int iTwo=0,iThree=0,iFive=0,i,iNum;
	for(i=10;i<1500;i++)
	{
		while(2*iUgly[iTwo]<=iUgly[i-1])iTwo++;
		while(3*iUgly[iThree]<=iUgly[i-1])iThree++;
		while(5*iUgly[iFive]<=iUgly[i-1])iFive++;
		if(2*iUgly[iTwo]<=3*iUgly[iThree] && 2*iUgly[iTwo]<=5*iUgly[iFive])
			iUgly[i]=2*iUgly[iTwo++];
		else if(3*iUgly[iThree]<=2*iUgly[iTwo] && 3*iUgly[iThree]<=5*iUgly[iFive])
			iUgly[i]=3*iUgly[iThree++];
		else if(5*iUgly[iFive]<=2*iUgly[iTwo] && 5*iUgly[iFive]<=3*iUgly[iThree])
			iUgly[i]=5*iUgly[iFive++];
	}
	while(cin>>iNum){if(iNum==0)break;cout<<iUgly[iNum-1]<<endl;}
}