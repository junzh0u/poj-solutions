#include<iostream.h>

/*

int reJosephus(int,long int);

int main()
{
	int iInput;
	long int iAttempt;
	while(1)
	{
		cin>>iInput;
	//	for(iInput=1;iInput<14;iInput++)
	//	{
		if(iInput)
		{
			for(iAttempt=iInput+1;;iAttempt++)
			{
				if(reJosephus(iInput,iAttempt))
					break;
			}
			cout<<iInput<<'\t'<<iAttempt<<endl;
		}
		else
			break;
	//	}
	//	break;
	}
	return 0;
}

int reJosephus(int iNumHalf,long int iDis)
{
	int iChild[26];
	int i,j,iLeaveNo=1,iSub=-1;
	for(i=0;i<2*iNumHalf;i++)
		iChild[i]=i+1;
	while(1)
	{
		for(j=0;j<iDis;)
		{
			iSub=(iSub+1)%(2*iNumHalf);
			if(iChild[iSub])
				j++;
		}
		if(iLeaveNo==iNumHalf+1)
		{
			return 1;
			break;
		}
		if(iSub<iNumHalf)
		{
			return 0;
			break;
		}
		iChild[iSub]=0;
		iLeaveNo++;
	}
}

*/

int main()
{
	long int iResult[13]={2,7,5,30,169,441,1872,7632,1740,93313,459901,1358657,2504881};
	int iCode;
	while(1)
	{
		cin>>iCode;
		if(iCode)
			cout<<iResult[iCode-1]<<endl;
		else
			break;
	}
	return 0;
}