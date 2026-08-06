#include<iostream.h>

void main()
{
	char cSeq[60];
	bool bTan[26],bWait[26];
	int iBedNum,iTanNum,iLeave,i;
	while(cin>>iBedNum)
	{
		if(!iBedNum)
			break;
		cin>>cSeq;
		iLeave=iTanNum=0;
		for(i=0;i<26;i++)
			bTan[i]=bWait[i]=false;
		for(i=0;cSeq[i]!='\0';i++)
		{
			if(bTan[cSeq[i]-'A'])
				iTanNum--;
			else if(bWait[cSeq[i]-'A'])
				iLeave++;
			else if(iTanNum==iBedNum)
				bWait[cSeq[i]-'A']=true;
			else
			{
				bTan[cSeq[i]-'A']=true;
				iTanNum++;
			}
		}
		if(iLeave)
			cout<<iLeave<<" customer(s) walked away."<<endl;
		else
			cout<<"All customers tanned successfully."<<endl;
	}
}