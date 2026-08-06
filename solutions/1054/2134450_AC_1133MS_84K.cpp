#include<stdio.h>
#include<stdlib.h>

struct Coordinate{int iRow,iCol;};

int CoordinateCompare(const void* pLeft,const void* pRight)
{
	if(((Coordinate*)pLeft)->iRow!=((Coordinate*)pRight)->iRow)
		return ((Coordinate*)pLeft)->iRow-((Coordinate*)pRight)->iRow;
	else return ((Coordinate*)pLeft)->iCol-((Coordinate*)pRight)->iCol;
}

void main()
{
	int iRowMax,iColMax,iFlatNum,i,j,iDamage,iDamageMax,iPtr,iRowDistance,iColDistance;
	Coordinate cNext,cFlat[5000];
	scanf("%d%d%d",&iRowMax,&iColMax,&iFlatNum);
	for(i=0;i<iFlatNum;i++)scanf("%d%d",&cFlat[i].iRow,&cFlat[i].iCol);
	qsort(cFlat,iFlatNum,sizeof(Coordinate),CoordinateCompare);
	iDamageMax=1;
	for(i=0;i<iFlatNum-1;i++)
		for(j=i+1;j<iFlatNum;j++)
		{
			iDamage=2;
			iRowDistance=cFlat[j].iRow-cFlat[i].iRow;iColDistance=cFlat[j].iCol-cFlat[i].iCol;
			if(cFlat[i].iRow-iRowDistance>0 && cFlat[i].iCol-iColDistance>0 && cFlat[i].iCol-iColDistance<=iColMax)continue;
			if(iRowDistance*(iDamageMax-1)+cFlat[j].iRow>iRowMax)continue;
			if(iColDistance*(iDamageMax-1)+cFlat[j].iCol<0 || iColDistance*(iDamageMax-1)+cFlat[j].iCol>iColMax)continue;
			cNext=cFlat[j];cNext.iRow+=iRowDistance;cNext.iCol+=iColDistance;
			iPtr=j+1;
			while(cNext.iRow<=iRowMax && cNext.iCol>0 && cNext.iCol<=iColMax)
			{
				while(CoordinateCompare(&cFlat[iPtr],&cNext)<0)iPtr++;
				if(CoordinateCompare(&cFlat[iPtr],&cNext)==0){iPtr++;iDamage++;}
				else{iDamage=0;break;}
				cNext.iRow+=iRowDistance;cNext.iCol+=iColDistance;
			}
			if(iDamage>=3 && iDamage>iDamageMax)iDamageMax=iDamage;
		}
	if(iDamageMax==1)iDamageMax=0;
	printf("%d\n",iDamageMax);
}