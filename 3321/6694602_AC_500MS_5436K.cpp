#include<cstdio>
#include<algorithm>

const int FORK_NUM_MAX=100000;
int forkNum;

struct Branch{
	int from,to;
};
const int BRANCH_NUM_MAX=2*(FORK_NUM_MAX-1);
int branchNum;
Branch branches[BRANCH_NUM_MAX];
int branchSection[FORK_NUM_MAX];

bool branchLess(const Branch &left,const Branch &right){
	if(left.from!=right.from)
		return left.from<right.from;
	return left.to<right.to;
}

int treeIndex[FORK_NUM_MAX],treeSize[FORK_NUM_MAX];
bool inTree[FORK_NUM_MAX];
int currentIndex;

int buildTree(int forkID){
	inTree[forkID]=true;
	treeSize[forkID]=1;
	for(int branchIndex=branchSection[forkID];branchIndex<branchNum && branches[branchIndex].from==forkID;branchIndex++){
		int nextForkID=branches[branchIndex].to;
		if(!inTree[nextForkID])
			treeSize[forkID]+=buildTree(nextForkID);
	}
	treeIndex[forkID]=currentIndex++;
	return treeSize[forkID];
}

int binaryIndexedAppleNum[FORK_NUM_MAX];
bool hasApple[FORK_NUM_MAX];

int lowbit(int x){
	return x&(x^(x-1));
}

void updateAppleNum(int forkIndex,int change){
	if(forkIndex>=forkNum)
		return;
	binaryIndexedAppleNum[forkIndex]+=change;
	forkIndex+=lowbit(forkIndex+1);
	updateAppleNum(forkIndex,change);
}

void grow(int forkIndex){
	hasApple[forkIndex]=true;
	updateAppleNum(forkIndex,1);
}

void pick(int forkIndex){
	hasApple[forkIndex]=false;
	updateAppleNum(forkIndex,-1);
}

int queryAppleNum(int forkIndex){
	int sum=0;
	while(forkIndex>=0){
		sum+=binaryIndexedAppleNum[forkIndex];
		forkIndex-=lowbit(forkIndex+1);
	}
	return sum;
}

int main(){
	scanf("%d",&forkNum);
	branchNum=2*(forkNum-1);
	for(int branchIndex=0;branchIndex<branchNum;branchIndex+=2){
		scanf("%d%d",&branches[branchIndex].from,&branches[branchIndex].to);
		branches[branchIndex].from--;
		branches[branchIndex].to--;
		branches[branchIndex+1].from=branches[branchIndex].to;
		branches[branchIndex+1].to=branches[branchIndex].from;
	}

	std::sort(branches,branches+branchNum,branchLess);
	branchSection[0]=0;
	for(int forkID=1;forkID<forkNum;forkID++){
		branchSection[forkID]=branchSection[forkID-1];
		while(branches[branchSection[forkID]].from!=forkID)
			branchSection[forkID]++;
	}

	std::fill(inTree,inTree+forkNum,false);
	currentIndex=0;
	buildTree(0);

	std::fill(binaryIndexedAppleNum,binaryIndexedAppleNum+forkNum,0);
	std::fill(hasApple,hasApple+forkNum,false);
	for(int forkIndex=0;forkIndex<forkNum;forkIndex++)
		grow(forkIndex);

	int operationNum;
	scanf("%d",&operationNum);
	while(operationNum--){
		char operationType;
		int forkID;
		scanf(" %c %d",&operationType,&forkID);
		forkID--;
		int forkIndex=treeIndex[forkID];
		if(operationType=='Q'){
			int appleNumInSubTree=queryAppleNum(forkIndex)-queryAppleNum(forkIndex-treeSize[forkID]);
			printf("%d\n",appleNumInSubTree);
		}
		else{
			if(hasApple[forkIndex])
				pick(forkIndex);
			else
				grow(forkIndex);
		}
	}
	return 0;
}