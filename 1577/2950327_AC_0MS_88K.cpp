#include<cstdio>

const int SIZE=100,INF=-1;
int level=0,nodeNum;
char leaves[SIZE][SIZE];
struct Node{
	char value;
	int left,right;
}tree[SIZE*SIZE];

void PushTree(int root,int leaf){
	if(root==leaf)
		return;
	if(tree[leaf].value<tree[root].value){
		if(tree[root].left==INF)
			tree[root].left=leaf;
		else
			PushTree(tree[root].left,leaf);
	}
	else{
		if(tree[root].right==INF)
			tree[root].right=leaf;
		else
			PushTree(tree[root].right,leaf);
	}
}

void PreOrder(int root){
	if(root==INF)
		return;
	printf("%c",tree[root].value);
	PreOrder(tree[root].left);
	PreOrder(tree[root].right);
}

void Solve(){
	nodeNum=0;
	for(int i=level-1;i>=0;i--)
		for(int j=0;leaves[i][j]!='\0';j++){
			tree[nodeNum].value=leaves[i][j];
			tree[nodeNum].left=tree[nodeNum].right=INF;
			PushTree(0,nodeNum++);
		}
	PreOrder(0);
	printf("\n");
}

int main(){
	while(gets(leaves[level])){
		if(leaves[level][0]=='*' || leaves[level][0]=='$'){
			Solve();
			if(leaves[level][0]=='$')
				break;
			level=0;
		}
		else
			level++;
	}
	return 0;
}