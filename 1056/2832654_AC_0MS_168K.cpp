#include<iostream>
#include<memory>
using namespace std;

void main()
{
	int caseNum=0;
	char code[11];
	bool suffix[2100],decodable;
	while(true)
	{
		decodable=true;
		memset(suffix,false,sizeof(suffix));
		while(cin>>code)
		{
			if(code[0]=='9')
				break;
			if(decodable)
			{
				int ptr=0;
				for(int i=0;code[i]!='\0';i++)
				{
					ptr=2*ptr+1+code[i]-'0';
					if(suffix[ptr])
					{
						decodable=false;
						break;
					}
				}
				suffix[ptr]=true;
			}
		}
		if(cin.eof())
			break;
		cout<<"Set "<<(++caseNum)<<" is"<<(decodable?" ":" not ")<<"immediately decodable"<<endl;
	}
}