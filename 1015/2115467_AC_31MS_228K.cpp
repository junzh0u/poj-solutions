#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
#define CANDMAX 200
#define JURYMAX 20
#define VALUEMAX 20

struct Value{int iDif,iSum;};
struct List{int iTotal,iQueue[JURYMAX];};

int iCandNum,iJuryNum;
Value vCandData[CANDMAX];
List lMemory[JURYMAX*VALUEMAX*2],lTemp[JURYMAX*VALUEMAX*2];

bool bUsed[CANDMAX];
void QueueToBool(int* iQueue,int iNum)
{
	int i;
	memset(bUsed,false,sizeof(bUsed));
	for(i=0;i<iNum;i++)
		bUsed[iQueue[i]]=true;
}

bool Input()
{
	int i,iPro,iDef;
	cin>>iCandNum>>iJuryNum;
	if(iCandNum==0)return false;
	memset(lMemory,0,sizeof(lMemory));
	for(i=0;i<iCandNum;i++)
	{
		cin>>iPro>>iDef;
		vCandData[i].iDif=iDef-iPro+VALUEMAX;
		vCandData[i].iSum=iDef+iPro+VALUEMAX;
		if(lMemory[vCandData[i].iDif].iTotal<vCandData[i].iSum)
		{
			lMemory[vCandData[i].iDif].iTotal=vCandData[i].iSum;
			lMemory[vCandData[i].iDif].iQueue[0]=i;
		}
	}
	return true;
}

void Print(List* plMemory,int iTotalDif)
{
	static int iCaseCount=0;
	int i;
	cout<<"Jury #"<<++iCaseCount<<endl;
	cout<<"Best jury has value "<<(plMemory[iTotalDif].iTotal-iTotalDif)/2
		<<" for prosecution and value "
		<<(plMemory[iTotalDif].iTotal+iTotalDif)/2-VALUEMAX*iJuryNum<<" for defence:"<<endl;
	sort(plMemory[iTotalDif].iQueue,plMemory[iTotalDif].iQueue+iJuryNum);
	for(i=0;i<iJuryNum;i++)
		cout<<" "<<plMemory[iTotalDif].iQueue[i]+1;
	cout<<"\n"<<endl;
}

void ClearList(List* plArray)
{
	int i;
	for(i=0;i<=iJuryNum*VALUEMAX*2;i++)
		plArray[i].iTotal=0;
}

void Solve()
{
	int iUsed,i,iNext;
	List *plMemory,*plTemp;
	plMemory=lMemory;plTemp=lTemp;
	for(iUsed=1;iUsed<iJuryNum;iUsed++)
	{
		ClearList(plTemp);
		for(i=0;i<=iUsed*VALUEMAX*2;i++)
		{
			if(plMemory[i].iTotal==0)continue;
			QueueToBool(plMemory[i].iQueue,iUsed);
			for(iNext=0;iNext<iCandNum;iNext++)
			{
				if(bUsed[iNext])continue;
				if(plTemp[i+vCandData[iNext].iDif].iTotal<plMemory[i].iTotal+vCandData[iNext].iSum)
				{
					plTemp[i+vCandData[iNext].iDif].iTotal=plMemory[i].iTotal+vCandData[iNext].iSum;
					memcpy(plTemp[i+vCandData[iNext].iDif].iQueue,plMemory[i].iQueue,sizeof(int)*iUsed);
					plTemp[i+vCandData[iNext].iDif].iQueue[iUsed]=iNext;
				}
			}
		}
		swap(plMemory,plTemp);
	}
	for(i=0;;i++)
		if(plMemory[iJuryNum*VALUEMAX+i].iTotal!=0 || plMemory[iJuryNum*VALUEMAX-i].iTotal!=0)
		{
			Print(plMemory,(plMemory[iJuryNum*VALUEMAX+i].iTotal>plMemory[iJuryNum*VALUEMAX-i].iTotal)?(iJuryNum*VALUEMAX+i):(iJuryNum*VALUEMAX-i));
			break;
		}
}

void main(){while(Input())Solve();}