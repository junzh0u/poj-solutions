#include<cstdio>
#include<string>

const int LIMIT=100,INF=-1;
char sequence[LIMIT+1];

int lengthMin[LIMIT+1][LIMIT+1],splitPoint[LIMIT+1][LIMIT+1];

int RegularLength(int begin,int end){
	if(end-begin<=0)
		return 0;
	if(end-begin==1)
		return 2;
	if(lengthMin[begin][end]!=INF)
		return lengthMin[begin][end];
	if((sequence[begin]=='(' && sequence[end-1]==')') || (sequence[begin]=='[' && sequence[end-1]==']')){
		lengthMin[begin][end]=RegularLength(begin+1,end-1)+2;
		splitPoint[begin][end]=INF;
	}
	else
		lengthMin[begin][end]=INT_MAX;
	for(int mid=begin+1;mid<end;mid++){
		int length=RegularLength(begin,mid)+RegularLength(mid,end);
		if(lengthMin[begin][end]>length){
			lengthMin[begin][end]=length;
			splitPoint[begin][end]=mid;
		}
	}
	return lengthMin[begin][end];
}

void PrintRegular(int begin,int end){
	if(end-begin<=0)
		return;
	if(end-begin==1){
		if(sequence[begin]=='(' || sequence[begin]==')')
			printf("()");
		else
			printf("[]");
		return;
	}
	if(splitPoint[begin][end]==INF){
		printf((sequence[begin]=='(')?"(":"[");
		PrintRegular(begin+1,end-1);
		printf((sequence[begin]=='(')?")":"]");
		return;
	}
	PrintRegular(begin,splitPoint[begin][end]);
	PrintRegular(splitPoint[begin][end],end);
}

int main(){
	while(gets(sequence)){
		int length=strlen(sequence);
		for(int i=0;i<=length;i++)
			for(int j=0;j<=length;j++)
				lengthMin[i][j]=INF;
		RegularLength(0,length);
		PrintRegular(0,length);
		printf("\n");
	}
	return 0;
}