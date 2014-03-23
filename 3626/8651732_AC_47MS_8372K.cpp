#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int X_MIN=-500,X_MAX=500,Y_MIN=-500,Y_MAX=500;
const int HEIGHT=X_MAX-X_MIN,WIDTH=Y_MAX-Y_MIN;
const int MUD_NUM_MAX=10000;
const int DIRECTION_NUM=4;
const int X_SHIFTS[DIRECTION_NUM]={0,0,1,-1};
const int Y_SHIFTS[DIRECTION_NUM]={1,-1,0,0};

int rowFromX(int x){
	return x-X_MIN;
}

int colFromY(int y){
	return y-Y_MIN;
}

struct Coord{
	int x,y;
};

Coord source={0,0},bessie;
bool mud[HEIGHT][WIDTH]={false};

void setMud(Coord coord){
	int row=rowFromX(coord.x),col=colFromY(coord.y);
	mud[row][col]=true;
}

bool isMud(Coord coord){
	int row=rowFromX(coord.x),col=colFromY(coord.y);
	return mud[row][col];
}

void input(){
	int mudNum;
	scanf("%d%d%d",&bessie.x,&bessie.y,&mudNum);
	for(int i=0;i<mudNum;i++){
		Coord coord;
		scanf("%d%d",&coord.x,&coord.y);
		setMud(coord);
	}
}

int step[HEIGHT][WIDTH];

void initializeStep(){
	for(int row=0;row<HEIGHT;row++)
		for(int col=0;col<WIDTH;col++)
			step[row][col]=-1;
}

void setStep(Coord coord,int value){
	int row=rowFromX(coord.x),col=colFromY(coord.y);
	step[row][col]=value;
}

int getStep(Coord coord){
	int row=rowFromX(coord.x),col=colFromY(coord.y);
	return step[row][col];
}

Coord queue[HEIGHT*WIDTH];
int begin=0,end=0;

int shortestDistance(){
	initializeStep();
	queue[end++]=source;
	setStep(source,0);
	while(begin<end){
		Coord current=queue[begin++];
		int nextStep=getStep(current)+1;
		for(int direction=0;direction<DIRECTION_NUM;direction++){
			int xShift=X_SHIFTS[direction],yShift=Y_SHIFTS[direction];
			Coord next={current.x+xShift,current.y+yShift};
			if(next.x==bessie.x && next.y==bessie.y)
				return nextStep;
			if(isMud(next) || getStep(next)>=0)
				continue;
			queue[end++]=next;
			setStep(next,nextStep);
		}
	}
	return -1;
}

int main(){
	input();
	printf("%d\n",shortestDistance());
	return 0;
}