#include<iostream.h>
#include<math.h>

void main()
{
	int iA,iB,iN;
	while(1)
	{
		cin>>iB>>iN;
		if(!iB && !iN)
			break;
		iA=(int)pow(iB,1.0/iN);
		if(fabs(pow(iA,iN)-iB)>fabs(pow(iA+1,iN)-iB))
			iA++;
		cout<<iA<<endl;
	}
}