#include<iostream.h>

void main()
{
	int iDataNum,i,*iSpeed,*iTime,iDis;
	cin>>iDataNum;
	while(iDataNum!=-1)
	{
		iDis=0;
		iSpeed=new int [iDataNum];
		iTime=new int [iDataNum];
		for(i=0;i<iDataNum;i++)
		{
			cin>>iSpeed[i]>>iTime[i];
			iDis+=iSpeed[i]*(i?(iTime[i]-iTime[i-1]):iTime[i]);
		}
		cout<<iDis<<" miles"<<endl;
		cin>>iDataNum;
	}
}