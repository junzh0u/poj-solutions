#include<iostream.h>
#include<math.h>

void main()
{
	unsigned long int iCaseNum,iProID,iNum,i,j,iCount;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		iCount=0;
		cin>>iProID>>iNum;
		for(j=2;j<sqrt(iNum*2);j++)
			//if( (j%2==0 && iNum%(j/2)==0 && (iNum/(j/2))%2==1 && (iNum/(j/2))>j) || (j%2!=0 && iNum%j==0 && iNum/j>=(j+1)/2) )
			if(2*iNum%j==0 && (2*iNum/j+1-j)%2==0)
				iCount++;
		cout<<iProID<<" "<<iCount<<endl;
	}
}