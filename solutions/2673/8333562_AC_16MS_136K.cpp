#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int ANIMAL_NUM_MAX=1000;

struct Animal{
	int distance,speed;
};

int timeLimit,handleSpeed,animalNum;
Animal animals[ANIMAL_NUM_MAX];

int main(){
	scanf("%d%d%d",&timeLimit,&handleSpeed,&animalNum);
	for(int i=0;i<animalNum;i++)
		scanf("%d%d",&animals[i].distance,&animals[i].speed);
	int handleTime=timeLimit;
	for(int i=0;i<animalNum;i++){
		int timeBeforeEaten=(animals[i].distance-1)/animals[i].speed;
		if(handleTime>timeBeforeEaten)
			handleTime=timeBeforeEaten;
	}
	if(handleTime!=0)
		handleTime+=(timeLimit-handleTime)/2;
	printf("%d\n",handleTime*handleSpeed);
	return 0;
}