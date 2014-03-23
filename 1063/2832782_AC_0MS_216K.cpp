#include<iostream>
using namespace std;

void main()
{
	int caseNum;
	cin>>caseNum;
	while((caseNum--)>0)
	{
		int trackLength,oddCount=0,evenCount=0,disk;
		cin>>trackLength;
		for(int i=0;i<trackLength;i++)
		{
			cin>>disk;
			if(disk==1)
			{
				if(i%2==0)
					evenCount++;
				else
					oddCount++;
			}
		}
		if(trackLength%2!=0 || abs(evenCount-oddCount)<2)
			cout<<"YES"<<endl;
		else
			cout<<"NO"<<endl;
	}
}