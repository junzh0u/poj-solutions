#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<algorithm>

const int RECT_NUM_MAX=5000;
const int Y_MIN=-10000,Y_MAX=10000;

struct Line{
	int x;
	int yBegin,yEnd;
	bool left;
	bool operator<(const Line& a){
		if(x!=a.x)
			return x<a.x;
		return left>a.left;
	}
#ifdef DEBUG
	void print() const{
		printf("%c\tx:%3d\ty:%3d->%3d\n",left?'+':'-',x,yBegin,yEnd);
	}
#endif
};

int rectNum;
Line lines[2*RECT_NUM_MAX];

struct SegmentTreeNode{
	int yBegin,yEnd,coverCount;
};

SegmentTreeNode nodes[2*(Y_MAX-Y_MIN+1)];

int leftChild(int root){
	return 2*root+1;
}

int rightChild(int root){
	return 2*root+2;
}

void buildTree(int rootIndex,int yBegin,int yEnd){
	nodes[rootIndex].yBegin=yBegin;
	nodes[rootIndex].yEnd=yEnd;
	nodes[rootIndex].coverCount=0;
	if(yEnd-yBegin<=1)
		return;
	int yMid=(yBegin+yEnd)/2;
	buildTree(leftChild(rootIndex),yBegin,yMid);
	buildTree(rightChild(rootIndex),yMid,yEnd);
}

void modifyNode(int index,int yBegin,int yEnd,int diff){
	if(nodes[index].yBegin>=yBegin && nodes[index].yEnd<=yEnd){
		nodes[index].coverCount+=diff;
		return;
	}
	int yMid=(nodes[index].yBegin+nodes[index].yEnd)/2;
	if(yBegin<yMid)
		modifyNode(leftChild(index),yBegin,yEnd,diff);
	if(yEnd>yMid)
		modifyNode(rightChild(index),yBegin,yEnd,diff);
}

void insertSegment(const Line &line){
#ifdef DEBUG
	printf("# Insert:\n");
	line.print();
#endif
	modifyNode(0,line.yBegin,line.yEnd,1);
}

void deleteSegment(const Line &line){
#ifdef DEBUG
	printf("# Delete:\n");
	line.print();
#endif
	modifyNode(0,line.yBegin,line.yEnd,-1);
}

int getLengthSum(int index){
	if(nodes[index].coverCount>0)
		return nodes[index].yEnd-nodes[index].yBegin;
	if(nodes[index].yEnd-nodes[index].yBegin<=1)
		return 0;
	return getLengthSum(leftChild(index))+getLengthSum(rightChild(index));
}

bool leftCovered(int index){
	if(nodes[index].coverCount>0)
		return true;
	if(nodes[index].yEnd-nodes[index].yBegin<=1)
		return false;
	return leftCovered(leftChild(index));
}

bool rightCovered(int index){
	if(nodes[index].coverCount>0)
		return true;
	if(nodes[index].yEnd-nodes[index].yBegin<=1)
		return false;
	return rightCovered(rightChild(index));
}

int getFragmentNum(int index){
	if(nodes[index].coverCount>0)
		return 1;
	if(nodes[index].yEnd-nodes[index].yBegin<=1)
		return 0;
	int result=getFragmentNum(leftChild(index))+getFragmentNum(rightChild(index));
	if(rightCovered(leftChild(index)) && leftCovered(rightChild(index)))
		result--;
	return result;
}

#ifdef DEBUG
void printNode(int index){
	if(nodes[index].coverCount>0){
		printf("{ %3d->%3d }:\t%d\n",nodes[index].yBegin,nodes[index].yEnd,nodes[index].coverCount);
		return;
	}
	if(nodes[index].yEnd-nodes[index].yBegin<=1)
		return;
	printNode(leftChild(index));
	printNode(rightChild(index));
}
#endif

int main(){
	scanf("%d",&rectNum);
	int yMin=INT_MAX,yMax=INT_MIN;
	for(int i=0;i<rectNum;i++){
		int xBegin,yBegin,xEnd,yEnd;
		scanf("%d%d%d%d",&xBegin,&yBegin,&xEnd,&yEnd);
		lines[2*i].x=xBegin;
		lines[2*i+1].x=xEnd;
		lines[2*i].left=true;
		lines[2*i+1].left=false;
		lines[2*i].yBegin=lines[2*i+1].yBegin=yBegin;
		lines[2*i].yEnd=lines[2*i+1].yEnd=yEnd;
		if(yMin>yBegin)
			yMin=yBegin;
		if(yMax<yEnd)
			yMax=yEnd;
	}
	std::sort(lines,lines+2*rectNum);
#ifdef DEBUG
	printf("# Lines:\n");
	for(int i=0;i<2*rectNum;i++)
		lines[i].print();
#endif
	buildTree(0,yMin,yMax);

	int lineIndex=0;
	int lengthSumPrev=0;
	int perimeter=0;
	for(int lineIndex=0;;){
		int x=lines[lineIndex].x;
		int lineNext=lineIndex;
		while(lineNext<2*rectNum && lines[lineNext].x==x){
			if(lines[lineNext].left)
				insertSegment(lines[lineNext]);
			else
				deleteSegment(lines[lineNext]);
			int lengthSum=getLengthSum(0);
			perimeter+=abs(lengthSum-lengthSumPrev);
#ifdef DEBUG
			printf("# Length Sum = %d (Prev: %d )\n",lengthSum,lengthSumPrev);
			printf("# Perimeter = %d\n",perimeter);
#endif
			lengthSumPrev=lengthSum;
			lineNext++;
		}
#ifdef DEBUG
		printf("# Segment Tree:\n");
		printNode(0);
#endif
		
		if(lineNext>=2*rectNum)
			break;
		int fragmentNum=getFragmentNum(0);
		int xGap=lines[lineNext].x-lines[lineIndex].x;
		perimeter+=fragmentNum*xGap*2;
#ifdef DEBUG
		printf("# Fragment count = %d\n",fragmentNum);
		printf("# X Gap = %d\n",xGap);
		printf("# Perimeter = %d\n",perimeter);
#endif
		lineIndex=lineNext;
	}

	printf("%d\n",perimeter);
	return 0;
}