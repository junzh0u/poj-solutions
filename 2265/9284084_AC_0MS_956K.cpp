#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int ID_MAX=100000;
int x[ID_MAX]={0},y[ID_MAX]={0};

int main(){
	for(int id=0;id+1<ID_MAX;id++){
		if(x[id]>=0 && x[id]+y[id]<0){
			x[id+1]=x[id]+1;
			y[id+1]=y[id];
		}
		else if(y[id]<=0 && x[id]+y[id]>=0){
			x[id+1]=x[id];
			y[id+1]=y[id]+1;
		}
		else if(x[id]>0 && y[id]>0){
			x[id+1]=x[id]-1;
			y[id+1]=y[id]+1;
		}
		else if(x[id]<=0 && x[id]+y[id]>0){
			x[id+1]=x[id]-1;
			y[id+1]=y[id];
		}
		else if(y[id]>0 && x[id]+y[id]<=0){
			x[id+1]=x[id];
			y[id+1]=y[id]-1;
		}
		else if(x[id]<0 && y[id]<=0){
			x[id+1]=x[id]+1;
			y[id+1]=y[id]-1;
		}
	}
	int id;
	while(scanf("%d",&id)!=EOF)
		printf("%d %d\n",x[id-1],y[id-1]);
	return 0;
}