#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

int regionID=0;

const int WIDTH_MAX=30,HEIGHT_MAX=30;
int width,height;
int elevations[WIDTH_MAX*HEIGHT_MAX];
int waterVolume;
double waterLevel;
int next;

int main(){
	while(true){
		regionID++;

		scanf("%d%d",&width,&height);
		if(width==0 && height==0)
			return 0;
		for(int i=0;i<width*height;i++)
			scanf("%d",elevations+i);
		std::sort(elevations,elevations+width*height);
		scanf("%d",&waterVolume);

		waterLevel=elevations[0];
		next=0;

		while(waterVolume){
			for(;next<width*height && elevations[next]<=waterLevel;next++);
			if(next==width*height || (elevations[next]-waterLevel)*next*100>=waterVolume){
				waterLevel+=waterVolume/(next*100.0);
				waterVolume=0;
			}
			else{
				waterVolume-=(elevations[next]-waterLevel)*next*100;
				waterLevel=elevations[next];
			}
		}
		printf("Region %d\n",regionID);
		printf("Water level is %.2f meters.\n",waterLevel);
		printf("%.2f percent of the region is under water.\n",100.0*next/(width*height));
	}
}