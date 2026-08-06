#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int HEIGHT_MAX=100,WIDTH_MAX=10,STATUS_NUM_MAX=60;
const char HILL='H',PLAIN='P';

int height,width;
int hillMap[HEIGHT_MAX];

int statusNum;
int statuses[STATUS_NUM_MAX];
int numInStatus[STATUS_NUM_MAX];

int cannonNum[3][STATUS_NUM_MAX][STATUS_NUM_MAX];
int cannonNumMax;

void dpsStatus(int col,int status){
	if(col>=width){
		statuses[statusNum++]=status;
		return;
	}
	dpsStatus(col+1,status);
	status|=(1<<col);
	dpsStatus(col+3,status);
}

void countStatus(){
	for(int i=0;i<statusNum;i++){
		int status=statuses[i];
		numInStatus[i]=0;
		while(status){
			numInStatus[i]+=status&1;
			status>>=1;
		}
	}
}

void generateStatuses(){
	dpsStatus(0,0);
	countStatus();
}

int main(){
	scanf("%d%d",&height,&width);
	for(int row=0;row<height;row++){
		char line[WIDTH_MAX+1];
		scanf("%s",line);
		hillMap[row]=0;
		for(int col=0;col<width;col++)
			if(line[col]==HILL)
				hillMap[row]|=(1<<col);
	}
#ifdef _DEBUG
	printf("# Hills:\n");
	for(int row=0;row<height;row++){
		printf("#\t");
		for(int col=0;col<width;col++)
			printf("%d",(hillMap[row]>>col)&1);
		printf("\n");
	}
#endif

	statusNum=0;
	generateStatuses();
#ifdef _DEBUG
	printf("# Statuses: (%d total)\n",statusNum);
	for(int current=0;current<statusNum;current++){
		printf("# %d(%d):\t",current+1,numInStatus[current]);
		for(int col=0;col<width;col++)
			printf("%d",(statuses[current]>>col)&1);
		printf("\n");
	}
#endif

	for(int current=0;current<statusNum;current++){
		int value=((statuses[current]&hillMap[0])?0:numInStatus[current]);
		for(int prev=0;prev<statusNum;prev++)
			cannonNum[0][current][prev]=value;
	}

	for(int row=1;row<height;row++){
		for(int current=0;current<statusNum;current++){
			for(int prev=0;prev<statusNum;prev++)
				cannonNum[row%2][current][prev]=0;
			if(statuses[current]&hillMap[row])
				continue;
			for(int prev=0;prev<statusNum;prev++){
				if(statuses[current]&statuses[prev])
					continue;
				for(int prevprev=0;prevprev<statusNum;prevprev++){
					if(statuses[current]&statuses[prevprev])
						continue;
					if(statuses[prev]&statuses[prevprev])
						continue;
					if(cannonNum[row%2][current][prev]<cannonNum[1-row%2][prev][prevprev]+numInStatus[current])
						cannonNum[row%2][current][prev]=cannonNum[1-row%2][prev][prevprev]+numInStatus[current];
				}
			}
		}
	}

	cannonNumMax=0;
	for(int current=0;current<statusNum;current++)
		for(int prev=0;prev<statusNum;prev++)
			if(cannonNumMax<cannonNum[(width-1)%2][current][prev])
				cannonNumMax=cannonNum[(width-1)%2][current][prev];
	printf("%d\n",cannonNumMax);

	return 0;
}