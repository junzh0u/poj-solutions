#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

#define DEBUG

const int NUMBER_LEN_MAX=10;
const int NODE_NUM_MAX=80000;
const int NONE=-1;

struct Node{
	bool leaf;
	int children[10];
};
Node nodes[NODE_NUM_MAX];
int nodeNum;

bool recordNumber(int root,char* number){
	if(nodes[root].leaf)
		return false;
	if(*number=='\0'){
		nodes[root].leaf=true;
		for(int i=0;i<10;i++)
			if(nodes[root].children[i]!=NONE)
				return false;
		return true;
	}
	int digit=*number-'0';
	if(nodes[root].children[digit]==NONE){
		nodes[nodeNum].leaf=false;
		for(int i=0;i<10;i++)
			nodes[nodeNum].children[i]=NONE;
		nodes[root].children[digit]=nodeNum++;
	}
	return recordNumber(nodes[root].children[digit],number+1);
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		nodes[0].leaf=false;
		for(int i=0;i<10;i++)
			nodes[0].children[i]=NONE;
		nodeNum=1;

		int numberNum;
		scanf("%d",&numberNum);
		bool consistent=true;
		while(numberNum-->0){
			char number[NUMBER_LEN_MAX+1];
			scanf("%s",number);
			if(consistent)
				consistent=recordNumber(0,number);
		}
		printf(consistent?"YES\n":"NO\n");
	}
	return 0;
}