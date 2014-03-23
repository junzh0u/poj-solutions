#include <iostream.h>

void main()
{
	int a,b,c,d,i,count=1;
	while(1)
	{
		cin>>a>>b>>c>>d;
		if(a+b+c+d==-4)
			break;
		a=a%23;
		b=b%28;
		c=c%33;
		for(i=1;;i++)
		{
			if(i%23==a && i%28==b && i%33==c && i>d)
			{	
				cout<<"Case "<<count<<": the next triple peak occurs in "<<i-d<<" days."<<endl;
				break;
			}
		}
		count++;
	}
}