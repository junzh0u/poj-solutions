#include<iostream.h>
#include<string.h>

const int FACTORIAL[10]={1,1,2,2*3,2*3*4,2*3*4*5,2*3*4*5*6,2*3*4*5*6*7,2*3*4*5*6*7*8,2*3*4*5*6*7*8*9};
bool bAnswer[1000001]={false};

void main()
{
	int i,j,iN;
	memset(bAnswer,false,sizeof(bAnswer));bAnswer[0]=true;
	for(i=0;i<=9;i++)
		for(j=1000000-FACTORIAL[i];j>=0;j--)
			if(bAnswer[j])
				bAnswer[j+FACTORIAL[i]]=true;
	bAnswer[0]=false;
	while(cin>>iN){if(iN<0)break;cout<<(bAnswer[iN]?"YES":"NO")<<endl;}
}