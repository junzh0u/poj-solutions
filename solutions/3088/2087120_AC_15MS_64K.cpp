#include<iostream.h>

unsigned int Factorial(unsigned int iExp){return (iExp==0)?1:(iExp*Factorial(iExp-1));}
unsigned int Choose(unsigned int iTotalNum,unsigned int iChooseNum){return Factorial(iTotalNum)/(Factorial(iChooseNum)*Factorial(iTotalNum-iChooseNum));}

unsigned int Partition(unsigned int iTotalNum,unsigned int iPartNum)
{
	unsigned int iReturn=0,i;
	if(iPartNum==1)
		return 1;
	for(i=1;i<=iTotalNum-iPartNum+1;i++)
		iReturn+=Choose(iTotalNum,i)*Partition(iTotalNum-i,iPartNum-1);
	//iReturn/=Factorial(iPartNum);
	return iReturn;
}

unsigned int ValidSequence(unsigned int iButtonNum)
{
	unsigned int iButtonUsed,iSequenceNum=0,iCombinationNum;
	for(iButtonUsed=1;iButtonUsed<=iButtonNum;iButtonUsed++)
		for(iCombinationNum=1;iCombinationNum<=iButtonUsed;iCombinationNum++)
		{
			//cout<<"Use:"<<iButtonUsed<<" - Comb:"<<iCombinationNum<<" - "<<Choose(iButtonNum,iButtonUsed)<<"*"<<Partition(iButtonUsed,iCombinationNum)<<endl;
			iSequenceNum+=Choose(iButtonNum,iButtonUsed)*Partition(iButtonUsed,iCombinationNum);
		}
	return iSequenceNum;
}

void main()
{
	unsigned int iCaseNum,iPretreat[11],i,iButtonNum;
	for(i=0;i<11;i++)
	{
		iPretreat[i]=ValidSequence(i+1);
		//cout<<iPretreat[i]<<endl<<endl;
	}
	//cout<<endl;
	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cin>>iButtonNum;
		cout<<i<<" "<<iButtonNum<<" "<<iPretreat[iButtonNum-1]<<endl;
	}
}