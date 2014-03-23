#include<iostream.h>
#include<math.h>

int calNum(int);

void main()
{
	int iDay;
	while(1)
	{
		cin>>iDay;
		if(iDay)
			cout<<iDay<<" "<<calNum(iDay)<<endl;
		else
			break;
	}
}

int calNum(int iDay)
{
	//double dN;
	int iN,iTotal;
	//dN=(sqrt(8*iDay+1)-1)/2;
	iN=(sqrt(8*iDay+1)-1)/2;
	iTotal=iN*(iN+1)*(2*iN+1)/6+(iDay-iN*(iN+1)/2)*(iN+1);
	//cout<<<<dN<<" "<<iN<<"\n";
	return iTotal;
}