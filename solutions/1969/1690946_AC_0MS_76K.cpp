#include<iostream.h>
#include<math.h>

void main()
{
	int iN,iColOdd;
	while(cin>>iN)
	{
		iColOdd=(sqrt(1+8*iN)+1)/4;
		if(iN==2*iColOdd*iColOdd-iColOdd)
			cout<<"TERM "<<iN<<" IS "<<1<<"/"<<2*iColOdd-1<<endl;
		else if(iN<=2*iColOdd*iColOdd+iColOdd)
			cout<<"TERM "<<iN<<" IS "<<iN-2*iColOdd*iColOdd+iColOdd<<"/"<<iColOdd+1-iN+2*iColOdd*iColOdd<<endl;
		else
			cout<<"TERM "<<iN<<" IS "<<3*iColOdd+2-iN+2*iColOdd*iColOdd<<"/"<<iN-2*iColOdd*iColOdd-iColOdd<<endl;
	}
}