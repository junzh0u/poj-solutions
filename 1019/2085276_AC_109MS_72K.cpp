#include<iostream.h>
#include<math.h>

const int TOTAL[5]={0,45,9045,1395495,189414495};

int SequenceLength(int iLimit)
{
	int iReturn,iExponent=log10(iLimit);
	iReturn=(iLimit-(int)pow(10,iExponent))*(iExponent+1);
	while(iExponent--)
		iReturn+=(iExponent+1)*9*(int)pow(10,iExponent);
	return iReturn;
}

int TotalLength(int iLimit)
{
	if(iLimit==1)
		return 0;
	else
		return SequenceLength(iLimit)+TotalLength(iLimit-1);
}

int FindDigit(int iLimit,int iPosition)
{
	int iRoot,i,iLen;
	for(iRoot=1;iRoot<=iLimit;iRoot++)
	{
		iLen=(int)log10(iRoot)+1;
		if(iPosition<=iLen)
			break;
		else
			iPosition-=iLen;
	}
	iRoot/=pow(10,iLen-iPosition);
	return iRoot%10;
}

void main()
{
	int iCaseNum,iPosition,iExponent,i,iTemp;
	//for(i=0;i<5;i++)
	//	cout<<pow(10,i)<<"\t"<<SequenceLength(pow(10,i))<<"\t"<<TotalLength(pow(10,i))<<endl;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iPosition;
		for(iExponent=4;iExponent>=0;iExponent--)
			if(iPosition>TOTAL[iExponent])
				break;
		iPosition-=TOTAL[iExponent];
		//cout<<"Exp: "<<iExponent<<"\tPos: "<<iPosition<<endl;
		for(i=pow(10,iExponent);;i++)
		{
			iTemp=SequenceLength(i+1);
			if(iPosition>iTemp)
				iPosition-=iTemp;
			else
				break;
		}
		//cout<<i<<" "<<iPosition<<" "<<;
		cout<<FindDigit(i,iPosition)<<endl;
	}
}