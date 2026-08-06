#include<iostream.h>

void main()
{
	int iLimit,iWidth,iHeight,iRowWidth,iRowMax,iHeightMax,iHeightSum;
	while(cin>>iLimit)
	{
		if(!iLimit)
			break;
		iRowWidth=iRowMax=iHeightSum=iHeightMax=0;
		while(cin>>iWidth>>iHeight)
		{
			if(iWidth==-1)
				break;
			if(iWidth+iRowWidth<=iLimit)
			{
				iRowWidth+=iWidth;
				if(iHeight>iHeightMax)
					iHeightMax=iHeight;
			}
			else
			{
				if(iRowWidth>iRowMax)
					iRowMax=iRowWidth;
				iRowWidth=iWidth;
				iHeightSum+=iHeightMax;
				iHeightMax=iHeight;
			}
		}
		if(iRowWidth>iRowMax)
			iRowMax=iRowWidth;
		iHeightSum+=iHeightMax;
		cout<<iRowMax<<" x "<<iHeightSum<<endl;
	}
}