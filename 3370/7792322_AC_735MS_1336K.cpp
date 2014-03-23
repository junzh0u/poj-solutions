#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>

const int NEIGH_MAX=100000,CHILD_MAX=100000,UNDEF=-1;

int child,neigh,sweet[NEIGH_MAX],remain[NEIGH_MAX+1],index[CHILD_MAX];

void print(int begin,int end){
	for(int i=begin;i<end;i++)
		printf("%d ",i+1);
	printf("\n");
}

void process(){
	for(int i=0;i<child;i++)
		index[i]=UNDEF;
	remain[0]=0;
	for(int end=1;end<=neigh;end++){
		remain[end]=(remain[end-1]+sweet[end-1])%child;
		if(remain[end]==0)
			return print(0,end);
		if(index[remain[end]]!=UNDEF)
			return print(index[remain[end]],end);
		index[remain[end]]=end;
	}
}

bool input(){
	scanf("%d%d",&child,&neigh);
	if(child==0 && neigh==0)
		return false;
	for(int i=0;i<neigh;i++)
		scanf("%d",sweet+i);
	return true;
}

int main(){
	while(input())
		process();
	return 0;
}