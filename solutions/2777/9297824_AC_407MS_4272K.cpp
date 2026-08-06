#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

struct Node{
	int first,last,color;
	bool leaf;
};

const int LENGTH_MAX=100000,COLOR_NUM_MAX=30;
const int NODE_NUM_MAX=3*LENGTH_MAX;
const int INITIAL_COLOR=1;

int length,colorNum,operationNum;
Node tree[NODE_NUM_MAX];

int leftChild(int father){return 2*father+1;}
int rightChild(int father){return 2*father+2;}

void build(int index,int first,int last){
	tree[index].first=first;
	tree[index].last=last;
	tree[index].color=INITIAL_COLOR;
	tree[index].leaf=true;
	if(first==last)
		return;
	int mid=(first+last)/2;
	build(leftChild(index),first,mid);
	build(rightChild(index),mid+1,last);
}

void paint(int index,int first,int last,int color){
	if(tree[index].last<first || last<tree[index].first)
		return;
	if(first<=tree[index].first && tree[index].last<=last){
		tree[index].color=color;
		tree[index].leaf=true;
		return;
	}
	if(tree[index].leaf){
		tree[leftChild(index)].color=tree[index].color;
		tree[leftChild(index)].leaf=true;
		tree[rightChild(index)].color=tree[index].color;
		tree[rightChild(index)].leaf=true;
		tree[index].leaf=false;
	}
	paint(leftChild(index),first,last,color);
	paint(rightChild(index),first,last,color);
}

int count(int index,int first,int last){
	if(tree[index].last<first || last<tree[index].first)
		return 0;
	if(tree[index].leaf)
		return 1<<tree[index].color;
	return count(leftChild(index),first,last) | count(rightChild(index),first,last);
}

int bitnum(int integer){
	int count=0;
	while(integer){
		if(integer&1)
			count++;
		integer>>=1;
	}
	return count;
}

#ifdef _DEBUG
void print(int index){
	printf("Node %d:\n\tRange:\t%d ~ %d\n\tColor:\t%d\n\tLeaf:\t%s\n",index,tree[index].first,tree[index].last,tree[index].color,(tree[index].leaf?"true":"false"));
	if(tree[index].first<tree[index].last){
		print(leftChild(index));
		print(rightChild(index));
	}
}
#endif

int main(){
	scanf("%d%d%d",&length,&colorNum,&operationNum);
	build(0,1,length);
#ifdef _DEBUG
	print(0);
#endif
	while(operationNum--){
		char operation[2];
		scanf("%s",operation);
		if(operation[0]=='C'){
			int first,last,color;
			scanf("%d%d%d",&first,&last,&color);
			if(first>last)
				std::swap(first,last);
			paint(0,first,last,color);
		}
		else{
			int first,last;
			scanf("%d%d",&first,&last);
			int state=count(0,first,last);
			printf("%d\n",bitnum(state));
		}
#ifdef _DEBUG
		print(0);
#endif
	}
	return 0;
}