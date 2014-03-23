#include<cstdio>
#include<algorithm>

const int INTEGER_NUM_MAX=100000,OPERATION_NUM_MAX=100000;

struct SegmentTreeNode{
	int begin,end;
	long long sum,increment;
};

int integerNum,operationNum;
int integers[INTEGER_NUM_MAX];
SegmentTreeNode segmentTree[INTEGER_NUM_MAX*3];

int leftSon(int index){
	return 2*index+1;
}

int rightSon(int index){
	return 2*index+2;
}

void createSegmentTree(int index,int begin,int end){
	segmentTree[index].begin=begin;
	segmentTree[index].end=end;
	if(begin+1==end)
		segmentTree[index].sum=integers[begin];
	else{
		int mid=(begin+end)/2;
		createSegmentTree(leftSon(index),begin,mid);
		createSegmentTree(rightSon(index),mid,end);
		segmentTree[index].sum=segmentTree[leftSon(index)].sum+segmentTree[rightSon(index)].sum;
	}
	segmentTree[index].increment=0;
}

long long sum(int index,int begin,int end){
	if(begin>=segmentTree[index].end || end<=segmentTree[index].begin)
		return 0;
	if(begin<=segmentTree[index].begin && end>=segmentTree[index].end)
		return segmentTree[index].sum;

	int sectionBegin=std::max(begin,segmentTree[index].begin),
		sectionEnd=std::min(end,segmentTree[index].end);
	long long incrementTotal=(sectionEnd-sectionBegin)*segmentTree[index].increment;
	return sum(leftSon(index),begin,end)+sum(rightSon(index),begin,end)+incrementTotal;
}

long long query(int begin,int end){
	return sum(0,begin,end);
}

void increase(int index,int begin,int end,int increment){
	if(begin>=segmentTree[index].end || end<=segmentTree[index].begin)
		return;

	int sectionBegin=std::max(begin,segmentTree[index].begin),
		sectionEnd=std::min(end,segmentTree[index].end);
	segmentTree[index].sum+=(sectionEnd-sectionBegin)*increment;

	if(begin<=segmentTree[index].begin && end>=segmentTree[index].end){
		segmentTree[index].increment+=increment;
		return;
	}
	increase(leftSon(index),begin,end,increment);
	increase(rightSon(index),begin,end,increment);
}

void add(int begin,int end,int increment){
	increase(0,begin,end,increment);
}

int main(){
	scanf("%d%d",&integerNum,&operationNum);
	for(int integerIndex=0;integerIndex<integerNum;integerIndex++)
		scanf("%d",integers+integerIndex);
	createSegmentTree(0,0,integerNum);
	while(operationNum--){
		char operationType;
		scanf(" %c",&operationType);
		if(operationType=='Q'){
			int lower,upper;
			scanf("%d%d",&lower,&upper);
			printf("%lld\n",query(lower-1,upper));
		}
		else if(operationType=='C'){
			int lower,upper,increment;
			scanf("%d%d%d",&lower,&upper,&increment);
			add(lower-1,upper,increment);
		}
	}
	return 0;
}