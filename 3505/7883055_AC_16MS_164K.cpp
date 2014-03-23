#define _CRT_SECURE_NO_WARNINGS

#undef _DEBUG

#include<cstdio>
#include<algorithm>

const int HEIGHT_MAX=50,LENGTH_MAX=50;
const int CAR_NUM_MAX=HEIGHT_MAX*(LENGTH_MAX-1);
const int EMPTY=-1;

int height,length;
int tower[HEIGHT_MAX][LENGTH_MAX];

int carNum;
int floorOfCar[CAR_NUM_MAX];

int time;

int posOfCar(int car,int floor){
	for(int pos=0;pos<length;pos++)
		if(tower[floor][pos]==car+1)
			return pos;
	return EMPTY;
}

void turnLeft(int floor,int shift){
	int temp[LENGTH_MAX];
	std::copy(tower[floor],tower[floor]+length,temp);
	for(int pos=0;pos<length;pos++)
		tower[floor][pos]=temp[(pos+shift)%length];
}

void carOut(int car){
	int floor=floorOfCar[car];
	time+=floor*20;
	int pos=posOfCar(car,floor);
	time+=std::min(pos,length-pos)*5;
	turnLeft(floor,pos);
	tower[floor][0]=EMPTY;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		carNum=0;
		scanf("%d%d",&height,&length);
		for(int floor=0;floor<height;floor++){
			for(int pos=0;pos<length;pos++){
				scanf("%d",tower[floor]+pos);
				if(tower[floor][pos]!=EMPTY){
					floorOfCar[tower[floor][pos]-1]=floor;
					if(carNum<tower[floor][pos])
						carNum=tower[floor][pos];
				}
			}
		}
		time=0;
#ifdef _DEBUG
		printf("# Tower:\n");
		for(int floor=0;floor<height;floor++){
			printf("#\t");
			for(int pos=0;pos<length;pos++)
				printf("%d ",tower[floor][pos]);
			printf("\n");
		}
#endif
		for(int car=0;car<carNum;car++){
			carOut(car);
#ifdef _DEBUG
			printf("# Tower:\n");
			for(int floor=0;floor<height;floor++){
				printf("#\t");
				for(int pos=0;pos<length;pos++)
					printf("%d ",tower[floor][pos]);
				printf("\n");
			}
			printf("# Time = %d\n",time);
#endif
		}
		printf("%d\n",time);
	}
	return 0;
}