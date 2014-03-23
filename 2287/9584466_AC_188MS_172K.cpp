#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int HORSE_NUM_MAX=1000;
int horseNum;
int tians[HORSE_NUM_MAX],kings[HORSE_NUM_MAX];

int race(int tian,int king){
	if(tian==king)
		return 0;
	return (tian<king)?-1:1;
}

int main(){
	while(true){
		scanf("%d",&horseNum);
		if(!horseNum)
			return 0;
		for(int i=0;i<horseNum;i++)
			scanf("%d",tians+i);
		for(int i=0;i<horseNum;i++)
			scanf("%d",kings+i);
		std::sort(tians,tians+horseNum);
		std::sort(kings,kings+horseNum);
		int winMax=-horseNum;
		for(int sacrifice=0;sacrifice<=horseNum;sacrifice++){
			int win=0;
			for(int i=0;i<sacrifice;i++)
				win+=race(tians[i],kings[horseNum-1-i]);
			for(int i=sacrifice;i<horseNum;i++)
				win+=race(tians[i],kings[i-sacrifice]);
			if(winMax<win)
				winMax=win;
		}
		printf("%d\n",winMax*200);
	}
}