#include<cstdio>

const int WIDTH_MAX=700,HEIGHT_MAX=700,AREA_WIDTH_MAX=100;
int map[HEIGHT_MAX][WIDTH_MAX];
int width,height,differenceMax,areaMax=0;

int main()
{
	scanf("%d%d%d",&width,&height,&differenceMax);
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			scanf("%d",&map[i][j]);
	for(int left=0;left<width;left++)
	{
		if((width-left)*height<=areaMax || AREA_WIDTH_MAX*height<=areaMax)
			break;
		int highest[HEIGHT_MAX],lowest[HEIGHT_MAX];
		for(int row=0;row<height;row++)
			highest[row]=lowest[row]=map[row][left];
		for(int right=left;right<width && right<left+AREA_WIDTH_MAX;right++)
		{
			int areaWidth=right-left+1;
			for(int row=0;row<height;row++)
			{
				if(highest[row]<map[row][right])
					highest[row]=map[row][right];
				else if(lowest[row]>map[row][right])
					lowest[row]=map[row][right];
			}
			if(areaWidth*height<=areaMax)
				continue;
			for(row=0;row<height;row++)
			//for(int row=0;row<height;row++)
				if(highest[row]-lowest[row]<=differenceMax)
				{
					if((height-row)*areaWidth<=areaMax)
						break;
					if(row>0 && highest[row]>=highest[row-1] && lowest[row]<=lowest[row-1])
						continue;
					int areaHighest=highest[row],areaLowest=lowest[row],bottom;
					for(bottom=row;bottom<height;bottom++)
					{
						if(areaHighest<highest[bottom])
							areaHighest=highest[bottom];
						if(areaLowest>lowest[bottom])
							areaLowest=lowest[bottom];
						if(areaHighest-areaLowest>differenceMax)
							break;
					}
					if(areaMax<(bottom-row)*areaWidth)
						areaMax=(bottom-row)*areaWidth;
				}
		}
	}
	printf("%d\n",areaMax);
	return 0;
}