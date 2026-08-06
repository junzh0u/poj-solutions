#include<iostream.h>

void main()
{
	short sDecSum,sHexSum,sD12Sum,sData;
	for(sData=2992;sData<10000;sData++)
	{
		sDecSum=sData/1000+(sData%1000)/100+(sData%100)/10+sData%10;
		sHexSum=sData/1728+(sData%1728)/144+(sData%144)/12+sData%12;
		sD12Sum=sData/4096+(sData%4096)/256+(sData%256)/16+sData%16;
		if(sDecSum==sHexSum && sDecSum==sD12Sum)
			cout<<sData<<endl;
	}
}