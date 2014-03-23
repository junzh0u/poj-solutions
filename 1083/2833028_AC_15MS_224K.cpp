#include<iostream>
using namespace std;

void main()
{
	int caseNum;
	cin>>caseNum;
	while((caseNum--)>0)
	{
		int moveNum;
		cin>>moveNum;
		int corridor[200];
		memset(corridor,0,sizeof(corridor));
		while((moveNum--)>0)
		{
			int from,to;
			cin>>from>>to;
			if(from>to)
				swap(from,to);
			for(int i=(from-1)/2;i<=(to-1)/2;i++)
				corridor[i]++;
		}
		int max=0;
		for(int i=0;i<200;i++)
			if(max<corridor[i])
				max=corridor[i];
		cout<<max*10<<endl;
	}
}