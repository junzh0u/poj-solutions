#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

struct Block{
	int height,quantity,altitude;
};

bool blockLessAltitude(const Block &left,const Block &right){
	return left.altitude<right.altitude;
}

const int TYPE_NUM_MAX=400,HEIGHT_MAX=100,QUANTITY_MAX=10,ALTITUDE_MAX=40000;

int typeNum;
Block blocks[TYPE_NUM_MAX];
bool possible[TYPE_NUM_MAX+1][ALTITUDE_MAX+1]={true};

int main(){
	scanf("%d",&typeNum);
	for(int i=0;i<typeNum;i++)
		scanf("%d%d%d",&blocks[i].height,&blocks[i].altitude,&blocks[i].quantity);
	std::sort(blocks,blocks+typeNum,blockLessAltitude);
	for(int type=0;type<typeNum;type++)
		for(int altitude=0;altitude<=ALTITUDE_MAX;altitude++)
			if(possible[type][altitude])
				for(int num=0;num<=blocks[type].quantity && altitude+num*blocks[type].height<=blocks[type].altitude;num++)
					possible[type+1][altitude+num*blocks[type].height]=true;
	for(int altitude=ALTITUDE_MAX;altitude>=0;altitude--){
		if(possible[typeNum][altitude]){
			printf("%d\n",altitude);
			break;
		}
	}
	return 0;
}