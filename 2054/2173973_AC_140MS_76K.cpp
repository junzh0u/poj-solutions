#include<stdio.h>
#include<string.h>
#define MAX 1000

struct Node
{double dValue;int iOrigin,iCount;Node *pnNext;Node *pnFather,*pnSon,*pnBrother;}
nTree[MAX],*pnRoot;
int iNodeNum;

Node* Max(Node* pnCur)
{
	Node *pnMax=pnCur,*pnPtr;
	if(pnCur->pnSon!=NULL)
	{pnPtr=Max(pnCur->pnSon);if(pnPtr->dValue>pnMax->dValue)pnMax=pnPtr;}
	if(pnCur->pnBrother!=NULL)
	{pnPtr=Max(pnCur->pnBrother);if(pnPtr->dValue>pnMax->dValue)pnMax=pnPtr;}
	return pnMax;
}

void main()
{
	int iRootCode,i,iFather,iSon,iCost,iTime;
	Node *pnPtr,**pPnPtr,*pnTemp;
	while(scanf("%d%d",&iNodeNum,&iRootCode) && iNodeNum!=0)
	{
		iCost=0;
		pnRoot=&nTree[iRootCode-1];
		for(i=0;i<iNodeNum;i++)
		{
			scanf("%d",&nTree[i].iOrigin);nTree[i].dValue=nTree[i].iOrigin;nTree[i].iCount=1;
			nTree[i].pnNext=nTree[i].pnFather=nTree[i].pnSon=nTree[i].pnBrother=NULL;
		}
		for(i=0;i<iNodeNum-1;i++)
		{
			scanf("%d%d",&iFather,&iSon);
			nTree[iSon-1].pnFather=&nTree[iFather-1];
			for(pPnPtr=&(nTree[iFather-1].pnSon);*pPnPtr!=NULL;pPnPtr=&((*pPnPtr)->pnBrother));
			*pPnPtr=&nTree[iSon-1];
		}
		while(pnRoot->pnSon!=NULL)
		{
			pnPtr=Max(pnRoot->pnSon);
			pnPtr->pnFather->dValue*=pnPtr->pnFather->iCount;
			pnPtr->pnFather->dValue+=pnPtr->dValue*pnPtr->iCount;
			pnPtr->pnFather->iCount+=pnPtr->iCount;
			pnPtr->pnFather->dValue/=pnPtr->pnFather->iCount;
			for(pnTemp=pnPtr->pnFather;pnTemp->pnNext!=NULL;pnTemp=pnTemp->pnNext);
			pnTemp->pnNext=pnPtr;
			if(pnPtr==pnPtr->pnFather->pnSon)pnPtr->pnFather->pnSon=pnPtr->pnBrother;
			else
			{
				for(pnTemp=pnPtr->pnFather->pnSon;pnTemp->pnBrother!=pnPtr;pnTemp=pnTemp->pnBrother);
				pnTemp->pnBrother=pnPtr->pnBrother;
			}
			if(pnPtr->pnSon!=NULL)
			{
				pnPtr->pnSon->pnFather=pnPtr->pnFather;
				for(pPnPtr=&(pnPtr->pnFather->pnSon);*pPnPtr!=NULL;pPnPtr=&((*pPnPtr)->pnBrother));
				*pPnPtr=pnPtr->pnSon;
			}
			for(pnTemp=pnPtr->pnSon;pnTemp!=NULL;pnTemp=pnTemp->pnBrother)
				pnTemp->pnFather=pnPtr->pnFather;
		}
		for(pnPtr=pnRoot,iTime=1;pnPtr!=NULL;pnPtr=pnPtr->pnNext,iTime++)
			iCost+=pnPtr->iOrigin*iTime;
		printf("%d\n",iCost);
	}
}