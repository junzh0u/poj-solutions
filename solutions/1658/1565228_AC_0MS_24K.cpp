#include<iostream.h>

void main()
{
	int iArrayNum,Array[5],i,j;
	cin>>iArrayNum;
	for(i=0;i<iArrayNum;i++)
	{
		for(j=0;j<4;j++)
			cin>>Array[j];
		if(Array[0]+Array[2]==2*Array[1])
			Array[4]=2*Array[3]-Array[2];
		else
			Array[4]=Array[3]*Array[3]/Array[2];
		for(j=0;j<5;j++)
			cout<<Array[j]<<" ";
		cout<<"\n";
	}
}