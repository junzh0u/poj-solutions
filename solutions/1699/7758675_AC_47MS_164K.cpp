#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<cstring>
#include<algorithm>

const int SEG_NUM_MAX=10,SEG_LEN_MAX=20;

int segmentNum;
char segments[SEG_NUM_MAX][SEG_LEN_MAX+1];
int lengthIncrement[SEG_NUM_MAX][SEG_NUM_MAX];

bool match(char* left,char* right){
	while(true){
		if(*left=='\0' || *right=='\0')
			return true;
		if(*left!=*right)
			return false;
		left++;
		right++;
	}
}

int increment(char* left,char* right){
	for(int shift=std::max((int)(strlen(left)-strlen(right)),0);;shift++)
		if(match(left+shift,right)){	
#ifdef DEBUG
			printf("%s matches %s\n",left+shift,right);
#endif
			return shift+strlen(right)-strlen(left);
		}
}

bool used[SEG_NUM_MAX];
int shortestLength;

void searchShortestLength(int length,int usedNum,int lastID){
	if(length>=shortestLength)
		return;
	if(usedNum==segmentNum){
		shortestLength=length;
		return;
	}
	for(int id=0;id<segmentNum;id++)
		if(!used[id]){
			used[id]=true;
			if(lastID==-1)
				searchShortestLength(strlen(segments[id]),usedNum+1,id);
			else
				searchShortestLength(length+lengthIncrement[lastID][id],usedNum+1,id);
			used[id]=false;
		}
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d",&segmentNum);
		for(int i=0;i<segmentNum;i++)
			scanf("%s",segments[i]);
		for(int leftID=0;leftID<segmentNum;leftID++)
			for(int rightID=0;rightID<segmentNum;rightID++)
				lengthIncrement[leftID][rightID]=increment(segments[leftID],segments[rightID]);
#ifdef DEBUG
		for(int i=0;i<segmentNum;i++){
			for(int j=0;j<segmentNum;j++)
				printf("%2d",lengthIncrement[i][j]);
			printf("\n");
		}
#endif
		std::fill(used,used+segmentNum,false);
		shortestLength=INT_MAX;
		searchShortestLength(0,0,-1);
		printf("%d\n",shortestLength);
	}
	return 0;
}