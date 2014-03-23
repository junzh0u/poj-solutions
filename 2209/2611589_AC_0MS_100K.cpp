#include<iostream.h>
#include<math.h>

void main()
{
	int sonNum,exponent;
	cin>>sonNum>>exponent;
	int chance=0,potential,power;
	for(int i=0;i<sonNum;i++)
	{
		cin>>potential;
		power=pow(potential,exponent);
		if(power>0)
			chance+=power;
	}
	cout<<chance<<endl;
}