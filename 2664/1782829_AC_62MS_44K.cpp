//#include<iostream.h>
#include<stdio.h>

void main()
{
	int iCourChosNum,iCourChos[100],iCataNum,iCataCourNum,iCataMin,iCataChos,iCataTemp,i;
	bool bDegree;
	while(1)
	{
		scanf("%d",&iCourChosNum);
		if(!iCourChosNum)
			break;
		scanf("%d",&iCataNum);
		for(i=0;i<iCourChosNum;i++)
			scanf("%d",&iCourChos[i]);
		bDegree=true;
		while(iCataNum--)
		{
			scanf("%d%d",&iCataCourNum,&iCataMin);
			iCataChos=0;
			while(iCataCourNum--)
			{
				scanf("%d",&iCataTemp);
				if(iCataChos<iCataMin && bDegree)
					for(i=0;i<iCourChosNum;i++)
						if(iCataTemp==iCourChos[i])
						{
							iCataChos++;
							break;
						}
			}
			if(iCataChos<iCataMin)
				bDegree=false;
		}
		printf("%s\n",bDegree?("yes"):("no"));
	}
}
