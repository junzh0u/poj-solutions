#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<algorithm>
#include<functional>
using namespace std;

const int COLOR_NUM_MAX=12,KIT_SIZE=50;

int colorNum,colors[COLOR_NUM_MAX],gray,remain[COLOR_NUM_MAX],kitNum;

void makeGray(){
	for(int i=0;i<3;i++)
		remain[i]--;
	gray--;
}

void oneMoreKit(){
	kitNum++;
	for(int i=0;i<colorNum;i++)
		remain[i]+=KIT_SIZE;
}

bool enoughColor(){
	for(int i=0;i<3;i++)
		if(remain[i]<=0)
			return false;
	return true;
}

void meetGray(){
	while(gray){
		sort(remain,remain+colorNum,greater<int>());
		if(!enoughColor())
			oneMoreKit();
		makeGray();
	}
}

void meetColors(){
	kitNum=(*max_element(colors,colors+colorNum)+KIT_SIZE-1)/KIT_SIZE;
	for(int i=0;i<colorNum;i++)
		remain[i]=kitNum*KIT_SIZE-colors[i];
}

bool input(){
	scanf("%d",&colorNum);
	if(colorNum==0)
		return false;
	for(int i=0;i<colorNum;i++)
		scanf("%d",colors+i);
	scanf("%d",&gray);
	return true;
}

int main(){
	while(input()){
		meetColors();
		meetGray();
		printf("%d\n",kitNum);
	}
	return 0;
}