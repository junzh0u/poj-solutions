#include<iostream.h>
#include<math.h>

void main()
{
	int iSpeed,iAngle,iDistance;double dResult;
	while(cin>>iSpeed>>iAngle>>iDistance && iSpeed!=0)
	{
		dResult=(exp(iAngle*acos(-1)/(180.0*sqrt(3))+log(iDistance))-iDistance)/iSpeed;
		if(dResult>=10000.0)cout<<"God help me!"<<endl;
		else cout<<int(dResult+0.5)<<endl;
	}
}