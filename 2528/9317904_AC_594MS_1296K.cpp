#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<map>
#include<algorithm>

struct Poster{
	int first,last;
};

const int POSTER_NUM_MAX=10000,NONE=-1;
const int POS_NUM_MAX=2*POSTER_NUM_MAX;

int posterNum;
Poster posters[POSTER_NUM_MAX];
std::map<int,int> idOfPos;
int visiblePoster[POS_NUM_MAX];
bool visible[POSTER_NUM_MAX];

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d",&posterNum);
		idOfPos.clear();
		for(int i=0;i<posterNum;i++){
			scanf("%d%d",&posters[i].first,&posters[i].last);
			idOfPos[posters[i].first]=0;
			idOfPos[posters[i].last]=0;
		}
		int id=0;
		for(std::map<int,int>::iterator iter=idOfPos.begin();iter!=idOfPos.end();iter++)
			iter->second=id++;
		for(int i=0;i<posterNum;i++){
			posters[i].first=idOfPos[posters[i].first];
			posters[i].last=idOfPos[posters[i].last];
		}
#ifdef _DEBUG
		printf("#\tPOS\tID\n");
		for(std::map<int,int>::iterator iter=idOfPos.begin();iter!=idOfPos.end();iter++)
			printf("#\t%d\t%d\n",iter->first,iter->second);
		printf("$\tPosters:\n");
		for(int i=0;i<posterNum;i++)
			printf("$\t%d -> %d\n",posters[i].first,posters[i].last);
#endif
		std::fill(visiblePoster,visiblePoster+idOfPos.size(),NONE);
		for(int i=0;i<posterNum;i++)
			for(int id=posters[i].first;id<=posters[i].last;id++)
				visiblePoster[id]=i;
		std::fill(visible,visible+posterNum,false);
		for(unsigned id=0;id<idOfPos.size();id++)
			if(visiblePoster[id]!=NONE)
				visible[visiblePoster[id]]=true;
		int visibleCount=0;
		for(int i=0;i<posterNum;i++)
			if(visible[i])
				visibleCount++;
		printf("%d\n",visibleCount);
	}
	return 0;
}