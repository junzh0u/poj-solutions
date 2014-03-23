#include<iostream.h>
#include<math.h>
#include<string.h>

int MEMORY[1200][14][200],PRIMESERIAL[1200];
bool ISPRIME[1200]={false};
void Initialize()
{
	int i,j,iPtr=0;
	memset(MEMORY,-1,sizeof(MEMORY));
	memset(ISPRIME,true,sizeof(ISPRIME));
	ISPRIME[0]=ISPRIME[1]=false;
	for(i=2;i<600;i++)
		if(ISPRIME[i])
			for(j=2;i*j<1200;j++)
				ISPRIME[i*j]=false;
	for(i=0;i<1200;i++)
		if(ISPRIME[i])
			PRIMESERIAL[i]=iPtr++;
}

int Combination(int iSum,int iPrimeNum,int iLeast)
{
	int i,iReturn=0;
	if(iPrimeNum==1)
		return ISPRIME[iSum];
	if(MEMORY[iSum][iPrimeNum][PRIMESERIAL[iLeast]]!=-1)
		return MEMORY[iSum][iPrimeNum][PRIMESERIAL[iLeast]];
	for(i=iLeast+1;i<iSum/iPrimeNum;i++)
		if(ISPRIME[i])
			iReturn+=Combination(iSum-i,iPrimeNum-1,i);
	MEMORY[iSum][iPrimeNum][PRIMESERIAL[iLeast]]=iReturn;
	return iReturn;
}

void main()
{
	int iSum,iPrimeNum;
	Initialize();
	while(cin>>iSum>>iPrimeNum)
	{
		if(iSum==0)
			break;
		cout<<Combination(iSum,iPrimeNum,0)<<endl;
	}
}