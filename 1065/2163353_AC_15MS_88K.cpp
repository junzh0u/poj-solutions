#include<iostream.h>
#include<stdlib.h>

struct Stick{int iLength,iWidth;bool bProcessed;} sList[5000];

int StickComp(const void* pLeft,const void* pRight)
{
	if(((Stick*)pLeft)->iLength!=((Stick*)pRight)->iLength)
		return ((Stick*)pRight)->iLength-((Stick*)pLeft)->iLength;
	return ((Stick*)pRight)->iWidth-((Stick*)pLeft)->iWidth;
}

void main()
{
	int iCaseNum,iStickNum,i,j,iTime,iTemp;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iStickNum;
		for(i=0;i<iStickNum;i++)
		{
			cin>>sList[i].iLength>>sList[i].iWidth;
			sList[i].bProcessed=false;
		}
		qsort(sList,iStickNum,sizeof(Stick),StickComp);
		iTime=0;
		//for(i=0;i<iStickNum;i++)
			//cout<<sList[i].iLength<<" "<<sList[i].iWidth<<endl;
		for(i=0;i<iStickNum;i++)
			if(!sList[i].bProcessed)
			{
				//cout<<sList[i].iLength<<" "<<sList[i].iWidth<<endl;
				iTime++;
				iTemp=i;
				for(j=i+1;j<iStickNum;j++)
					if(!sList[j].bProcessed && sList[j].iWidth<=sList[iTemp].iWidth)
					{
						//cout<<"* "<<sList[j].iLength<<" "<<sList[j].iWidth<<endl;
						sList[j].bProcessed=true;
						iTemp=j;
					}
			}
		cout<<iTime<<endl;
	}
}