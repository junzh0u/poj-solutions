#include<cstdio>
#include<algorithm>

const int SEQUENCE_LEN_MAX=100000;
int sequenceNum;
int sequence[SEQUENCE_LEN_MAX];

struct Section{
	int begin,end,value;
};
const int SECTION_NUM_MAX=SEQUENCE_LEN_MAX;
int sectionNum;
Section sections[SECTION_NUM_MAX];

void splitIntoSections(){
	sectionNum=0;
	sections[0].begin=0;
	sections[0].value=sequence[0];
	for(int sequenceIndex=1;sequenceIndex<sequenceNum;sequenceIndex++){
		if(sequence[sequenceIndex]!=sequence[sequenceIndex-1]){
			sections[sectionNum].end=sequenceIndex;
			sectionNum++;
			sections[sectionNum].begin=sequenceIndex;
			sections[sectionNum].value=sequence[sequenceIndex];
		}
	}
	sections[sectionNum++].end=sequenceNum;
}

int binarySearch(int begin,int end,int value){
	if(end-begin==1)
		return begin;
	int mid=(begin+end)/2;
	if(sections[mid].value==value)
		return mid;
	if(sections[mid].value>value)
		return binarySearch(begin,mid,value);
	return binarySearch(mid+1,end,value);
}

int findSectionIndex(int value){
	return binarySearch(0,sectionNum,value);
}

struct SegmentTreeNode{
	int begin,end,max;
};
const int SEGMENT_TREE_SIZE_MAX=SECTION_NUM_MAX*4;
SegmentTreeNode segmentTree[SEGMENT_TREE_SIZE_MAX];

int leftChild(int index){
	return 2*index+1;
}

int rightChild(int index){
	return 2*index+2;
}

int buildSegmentTree(int begin,int end,int rootIndex){
	segmentTree[rootIndex].begin=begin;
	segmentTree[rootIndex].end=end;
	if(end-begin==1)
		segmentTree[rootIndex].max=sections[begin].end-sections[begin].begin;
	else{
		int mid=(begin+end)/2;
		segmentTree[rootIndex].max=std::max(buildSegmentTree(begin,mid,leftChild(rootIndex)),
											buildSegmentTree(mid,end,rightChild(rootIndex)));
	}
	return segmentTree[rootIndex].max;
}

int querySegmentTree(int begin,int end,int rootIndex){
	if(begin>=end)
		return 0;

	if(begin>=segmentTree[rootIndex].end || end<=segmentTree[rootIndex].begin)
		return 0;
	if(begin<=segmentTree[rootIndex].begin && end>=segmentTree[rootIndex].end)
		return segmentTree[rootIndex].max;
	return std::max(querySegmentTree(begin,end,leftChild(rootIndex)),
					querySegmentTree(begin,end,rightChild(rootIndex)));
}

int main(){
	while(true){
		scanf("%d",&sequenceNum);
		if(sequenceNum==0)
			return 0;
		int queryNum;
		scanf("%d",&queryNum);
		for(int sequenceIndex=0;sequenceIndex<sequenceNum;sequenceIndex++)
			scanf("%d",sequence+sequenceIndex);
		splitIntoSections();
		buildSegmentTree(0,sectionNum,0);
		while(queryNum--){
			int begin,end;
			scanf("%d%d",&begin,&end);
			begin--;
			if(sequence[begin]==sequence[end-1]){
				printf("%d\n",end-begin);
				continue;
			}
			int firstSectionIndex=findSectionIndex(sequence[begin]);
			int firstSectionLen=sections[firstSectionIndex].end-begin;

			int lastSectionIndex=findSectionIndex(sequence[end-1]);
			int lastSectionLen=end-sections[lastSectionIndex].begin;

			int midSectionLenMax=querySegmentTree(firstSectionIndex+1,lastSectionIndex,0);
			printf("%d\n",std::max(midSectionLenMax,std::max(firstSectionLen,lastSectionLen)));
		}
	}
}