#include<cstdio>
#include<cmath>

void main()
{
	const int MAX_SIZE=50000;
	int size,inversion,i;
	while(scanf("%d%d",&size,&inversion))
	{
		if(size<0)
			break;
		int length=ceil((1.0+sqrt(1.0+8.0*inversion))/2.0);
		int head=size-length+(inversion-(length-1)*(length-2)/2)+1;
		for(i=1;i<=size-length;i++)
			printf("%d ",i);
		printf("%d ",head);
		for(i=size;i>=size-length+1;i--)
			if(i!=head)
				printf("%d ",i);
		printf("\n");
	}
}