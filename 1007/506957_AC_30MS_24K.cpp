#include<iostream.h>

int main()
{
	int i,j,k,iCaseNum,iArrayLen,iInver[101],iTempInver,iSign[101],iTempSign;
	for(i=0;i<101;i++)
		iSign[i]=i;
	char cDNA[101][51];
	cin>>iArrayLen>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>cDNA[i];
		iInver[i]=0;
		for(j=0;j<iArrayLen;j++)
		{
			for(k=j+1;k<iArrayLen;k++)
			{
				if(cDNA[i][k]<cDNA[i][j])
					iInver[i]++;
			}
		}
	}
	for(i=0;i<iCaseNum;i++)
	{
		for(j=i;j<iCaseNum;j++)
		{
			if(iInver[j]<iInver[i])
			{
				iTempInver=iInver[i];
				iInver[i]=iInver[j];
				iInver[j]=iTempInver;
				iTempSign=iSign[i];
				iSign[i]=iSign[j];
				iSign[j]=iTempSign;
			}
		}
	}
	for(i=0;i<iCaseNum;i++)
		cout<<cDNA[iSign[i]]<<endl;
	return 0;
}