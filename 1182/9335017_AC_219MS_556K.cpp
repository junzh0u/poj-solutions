#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
//#include<algorithm>

const int ANIMAL_NUM_MAX=50000,SELF=0,EAT=1,EATEN=2;
int animalNum,sentenceNum,sentenceFakeNum,parent[ANIMAL_NUM_MAX],relationToParent[ANIMAL_NUM_MAX];

int root(int animal){
	if(parent[animal]==animal)
		return animal;
	int r=root(parent[animal]);
	relationToParent[animal]=(relationToParent[animal]+relationToParent[parent[animal]])%3;
	parent[animal]=r;
	return r;
}

bool fake(int relation,int left,int right){
	if(left>=animalNum || right>=animalNum)
		return true;
	if(left==right)
		return relation!=SELF;
	int rootLeft=root(left),rootRight=root(right);
	if(rootLeft==rootRight)
		return relation!=(relationToParent[left]-relationToParent[right]+3)%3;
	relation=(-relationToParent[left]+relation+relationToParent[right]+3)%3;
	parent[rootLeft]=rootRight;
	relationToParent[rootLeft]=relation;
	return false;
}

int main(){
	scanf("%d%d",&animalNum,&sentenceNum);
	for(int i=0;i<animalNum;i++){
		parent[i]=i;
		relationToParent[i]=SELF;
	}
	sentenceFakeNum=0;
	while(sentenceNum--){
		int relation,left,right;
		scanf("%d%d%d",&relation,&left,&right);
		if(fake(relation-1,left-1,right-1)){
#ifdef _DEBUG
			printf("Fake!\n");
#endif
			sentenceFakeNum++;
		}
	}
	printf("%d\n",sentenceFakeNum);
	return 0;
}