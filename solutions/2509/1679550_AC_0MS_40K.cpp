#include<iostream.h>

void main()
{
	int iCiga,iButt,iK;
	while(cin>>iCiga>>iK)
	{
		iButt=iCiga;
		while(iButt>=iK)
		{
			iCiga+=iButt/iK;
			iButt=iButt/iK+iButt%iK;
		}
		cout<<iCiga<<endl;
	}
}