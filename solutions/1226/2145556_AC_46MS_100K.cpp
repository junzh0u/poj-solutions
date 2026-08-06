#include<iostream.h>
#include<string.h>

bool Include(char* cLeft,char* cRight)
{
	bool bRet;
	int i,j;
	if(strlen(cLeft)<strlen(cRight))
		return false;
	for(i=0;i<=strlen(cLeft)-strlen(cRight);i++)
	{
		bRet=true;
		for(j=0;j<strlen(cRight);j++)
			if(cLeft[i+j]!=cRight[j])
			{
				bRet=false;
				break;
			}
		if(bRet)
			return true;
	}
	return false;
}

void main()
{
	char cArray[200][200],cTemp[200],cReverse[200];
	int iCaseNum,iStringNum,i,j,k;
	bool bFound;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iStringNum;
		for(i=0;i<iStringNum;i++)
			cin>>cArray[i];
		if(iStringNum==1)
		{
			cout<<strlen(cArray[0])<<endl;
			continue;
		}
		bFound=false;
		for(i=strlen(cArray[0]);i>=1;i--)
			for(j=0;j<=strlen(cArray[0])-i;j++)
			{
				for(k=0;k<i;k++)
					cReverse[i-k-1]=cTemp[k]=cArray[0][j+k];
				cTemp[i]=cReverse[i]='\0';
				bFound=true;
				for(k=1;k<iStringNum;k++)
					if(!Include(cArray[k],cTemp) && !Include(cArray[k],cReverse))
					{
						bFound=false;
						break;
					}
				if(bFound)
					goto OUT;
			}
OUT:
		cout<<i<<endl;
	}
}