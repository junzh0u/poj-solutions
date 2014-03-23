#include<iostream.h>
#include<math.h>

void main()
{
	const double PI=3.1415926;
	int iCaseNum,i;
	double dInDia,dBallDia,dDis;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>dInDia>>dBallDia>>dDis;
		cout<<(int)(PI/asin((dBallDia+dDis)/(dInDia-dBallDia)))<<endl;
	}
}