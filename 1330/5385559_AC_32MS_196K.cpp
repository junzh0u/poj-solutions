#include<cstdio>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int nodeNum;
		scanf("%d",&nodeNum);
		int *parents=new int[nodeNum];
		for(int i=0;i<nodeNum;i++)
			parents[i]=i;
		for(int i=0;i<nodeNum-1;i++){
			int parent,son;
			scanf("%d%d",&parent,&son);
			parents[son-1]=parent-1;
		}
		int left,right;
		scanf("%d%d",&left,&right);
		left--;
		right--;
		bool *ancestorOfLeft=new bool[nodeNum];
		for(int i=0;i<nodeNum;i++)
			ancestorOfLeft[i]=false;
		while(true){
			ancestorOfLeft[left]=true;
			if(parents[left]==left)
				break;
			left=parents[left];
		}
		while(true){
			if(ancestorOfLeft[right]){
				printf("%d\n",right+1);
				break;
			}
			right=parents[right];
		}
		delete[] ancestorOfLeft;
		delete[] parents;
	}
	return 0;
}