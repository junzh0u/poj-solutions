#include<cstdio>
#include<algorithm>
using namespace std;

struct Building{
	int begin,end,height;
};

const int BUILDING_NUM_MAX=40000;
Building buildings[BUILDING_NUM_MAX];
int buildingNum;

const int LOCATION_NUM_MAX=2*BUILDING_NUM_MAX;
int indexToLocation[LOCATION_NUM_MAX];
int locationNum;

int binarySearch(int begin,int end,int value){
	if(end-begin==0)
		return begin;
	int mid=(begin+end)/2;
	if(value==indexToLocation[mid])
		return mid;
	if(value<indexToLocation[mid])
		return binarySearch(begin,mid,value);
	return binarySearch(mid,end,value);
}

int locationToIndex(int location){
	return binarySearch(0,locationNum,location);
}

struct SegmentTreeNode{
	int begin,end,height;
	bool leaf;
};
SegmentTreeNode segmentTree[4*LOCATION_NUM_MAX];

int leftSon(int index){
	return index*2+1;
}

int rightSon(int index){
	return index*2+2;
}

void createSegmentTree(int begin,int end,int index){
	segmentTree[index].begin=begin;
	segmentTree[index].end=end;
	segmentTree[index].height=0;
	if(end-begin==1){
		segmentTree[index].leaf=true;
		return;
	}
	segmentTree[index].leaf=false;
	int mid=(begin+end)/2;
	createSegmentTree(begin,mid,leftSon(index));
	createSegmentTree(mid,end,rightSon(index));
}

void updateSegmentTree(int begin,int end,int height,int index){
	if(segmentTree[index].begin>=end || segmentTree[index].end<=begin)
		return;
	if(segmentTree[index].begin>=begin && segmentTree[index].end<=end){
		segmentTree[index].leaf=true;
		segmentTree[index].height=height;
		return;
	}
	if(segmentTree[index].leaf){
		segmentTree[index].leaf=false;
		segmentTree[leftSon(index)].height=segmentTree[index].height;
		segmentTree[leftSon(index)].leaf=true;
		segmentTree[rightSon(index)].height=segmentTree[index].height;
		segmentTree[rightSon(index)].leaf=true;
	}
	updateSegmentTree(begin,end,height,leftSon(index));
	updateSegmentTree(begin,end,height,rightSon(index));
}

long long area(int index){
	if(segmentTree[index].leaf)
		return	(long long)segmentTree[index].height*
				(indexToLocation[segmentTree[index].end]-indexToLocation[segmentTree[index].begin]);
	return area(leftSon(index))+area(rightSon(index));
}

bool bulidingLess(const Building &left,const Building &right){
	if(left.height!=right.height)
		return left.height<right.height;
	if(left.begin!=right.begin)
		return left.begin<right.begin;
	return left.end>right.end;
}

int main(){
	scanf("%d",&buildingNum);

	int locationsTemp[LOCATION_NUM_MAX];
	for(int index=0;index<buildingNum;index++){
		scanf("%d%d%d",&buildings[index].begin,&buildings[index].end,&buildings[index].height);
		locationsTemp[2*index]=buildings[index].begin;
		locationsTemp[2*index+1]=buildings[index].end;
	}
	sort(locationsTemp,locationsTemp+2*buildingNum);
	locationNum=0;
	indexToLocation[locationNum++]=locationsTemp[0];
	for(int index=1;index<2*buildingNum;index++)
		if(indexToLocation[index]!=indexToLocation[index-1])
			indexToLocation[locationNum++]=locationsTemp[index];

	sort(buildings,buildings+buildingNum,bulidingLess);
	createSegmentTree(0,locationNum-1,0);
	for(int index=0;index<buildingNum;index++){
		if(index>=1 && buildings[index].height==buildings[index-1].height &&
			buildings[index].end<=buildings[index-1].end)
			continue;
		int beginIndex=locationToIndex(buildings[index].begin);
		int endIndex=locationToIndex(buildings[index].end);
		updateSegmentTree(beginIndex,endIndex,buildings[index].height,0);
	}

	printf("%lld\n",area(0));
	return 0;
}