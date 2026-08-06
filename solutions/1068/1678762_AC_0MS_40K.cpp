#include<iostream.h>

void main()
{
	const int ARRAYSIZE=17;
	int iSeqP[20],iSeqW[20],iCaseNum,iDataNum,i,j,k,iPointStr,iLeft,iRight,iSpaceNum;
	char cString[ARRAYSIZE];
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iDataNum;
		iPointStr=0;
		for(j=0;j<iDataNum;j++)
		{
			cin>>iSeqP[j];
			for(k=0;k<(j?(iSeqP[j]-iSeqP[j-1]):iSeqP[j]);k++)
			{
				cString[iPointStr]='(';
				iPointStr++;
			}
			cString[iPointStr]=')';
			iPointStr++;
		}
		cString[iPointStr]='\0';
		for(j=0;;j++)
		{
			iRight=iPointStr;
			for(k=0;k<iPointStr;k++)
				if(cString[k]==')')
				{
					iRight=k;
					break;
				}
			if(iRight==iPointStr)
				break;
			for(k=0;k<iRight;k++)
				if(cString[k]=='(')
					iLeft=k;
			iSpaceNum=0;
			for(k=iLeft+1;k<iRight;k++)
				if(cString[k]==' ')
					iSpaceNum++;
			iSeqW[j]=iRight-iLeft-iSpaceNum/2;
			cString[iLeft]=cString[iRight]=' ';
		}
		for(k=0;k<j;k++)
			cout<<iSeqW[k]<<" ";
		cout<<endl;
	}
}