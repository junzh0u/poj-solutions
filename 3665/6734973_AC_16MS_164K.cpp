#include<cstdio>

int maxRatingIndex(int *ratings,int songNum){
	int result=0;
	for(int index=1;index<songNum;index++)
		if(ratings[result]<ratings[index])
			result=index;
	return result;
}

void play(int index,int *ratings,int songNum){
	printf("%d\n",index+1);
	int ratingEverage=ratings[index]/(songNum-1);
	int ratingRemain=ratings[index]%(songNum-1);
	for(int distributeIndex=0;distributeIndex<songNum;distributeIndex++)
		if(distributeIndex!=index){
			ratings[distributeIndex]+=ratingEverage;
			if(ratingRemain){
				ratings[distributeIndex]++;
				ratingRemain--;
			}
		}
	ratings[index]=0;
}

int main(){
	int songNum,playNum;
	scanf("%d%d",&songNum,&playNum);
	int *ratings=new int[songNum];
	for(int index=0;index<songNum;index++)
		scanf("%d",ratings+index);
	while(playNum--)
		play(maxRatingIndex(ratings,songNum),ratings,songNum);
	delete[] ratings;
	return 0;
}