#include<iostream.h>
#include<stdlib.h>

int COMPARE(const void* arg1,const void* arg2)
{
	//cout<<"Compare\nReturn "<<(*(int*)arg1)<<"-"<<(*(int*)arg2)<<endl;
	return ((*(int*)arg1)-(*(int*)arg2));
};

void main()
{
	int iCowNum,iCow[10000],i;
	cin>>iCowNum;
	for(i=0;i<iCowNum;i++)
		cin>>iCow[i];
	qsort((void*)iCow,iCowNum,sizeof(int),COMPARE);
	//for(i=0;i<iCowNum;i++)
		//cout<<iCow[i]<<" ";
	cout<<iCow[iCowNum/2]<<endl;
}