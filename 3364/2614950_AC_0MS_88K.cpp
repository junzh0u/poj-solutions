#include<iostream.h>

void main()
{
	while(true)
	{
		int row,column,corner;
		cin>>row>>column>>corner;
		if(row==0)
			break;
		cout<<((row-7)*(column-7)+corner)/2<<endl;
	}
}
