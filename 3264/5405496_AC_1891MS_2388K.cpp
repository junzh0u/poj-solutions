#include<cstdio>
#include<algorithm>
using namespace std;

struct Feature{
	int max,min;
};
const Feature INVALID={INT_MIN,INT_MAX};

struct SegmentTreeNode{
	int lower,upper;
	Feature feature;
};

const int COW_NUM_MAX=200000;
int cowNum,questionNum;
int height[COW_NUM_MAX];
SegmentTreeNode nodes[3*COW_NUM_MAX];

inline int LeftSon(int index){
	return index*2+1;
}

inline int RightSon(int index){
	return index*2+2;
}

inline Feature Merge(Feature left,Feature right){
	Feature result;
	result.max=max(left.max,right.max);
	result.min=min(left.min,right.min);
	return result;
}

void CreateSegmentTree(int index,int lower,int upper){
	nodes[index].lower=lower;
	nodes[index].upper=upper;
	if(upper-lower==1)
		nodes[index].feature.max=nodes[index].feature.min=height[lower];
	else{
		int mid=(lower+upper)/2;
		CreateSegmentTree(LeftSon(index),lower,mid);
		CreateSegmentTree(RightSon(index),mid,upper);
		nodes[index].feature=Merge(nodes[LeftSon(index)].feature,nodes[RightSon(index)].feature);
	}
}

Feature SearchSegmentTree(int index,int begin,int end){
	if(begin<=nodes[index].lower && end>=nodes[index].upper){
		return nodes[index].feature;
	}
	if(begin>=nodes[index].upper || end<=nodes[index].lower)
		return INVALID;
	return Merge(SearchSegmentTree(LeftSon(index),begin,end),SearchSegmentTree(RightSon(index),begin,end));
}

int main(){
	scanf("%d%d",&cowNum,&questionNum);
	for(int i=0;i<cowNum;i++)
		scanf("%d",height+i);
	CreateSegmentTree(0,0,cowNum);
	while(questionNum-->0){
		int begin,end;
		scanf("%d%d",&begin,&end);
		Feature feature(SearchSegmentTree(0,begin-1,end));
		printf("%d\n",feature.max-feature.min);
	}
	return 0;
}