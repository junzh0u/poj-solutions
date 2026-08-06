#include<cstdio>
#include<iostream>
#include<algorithm>
#include<string>
using namespace std;

const int TREAP_SIZE_MAX=50000,INF=-1;

struct Node{
	string label;
	int priority,left,right,parent;
}treap[TREAP_SIZE_MAX];

bool CompareByLabel(Node left,Node right){
	return left.label<right.label;
}

void PushBinaryTree(int current,int root){
	if(treap[current].label<treap[root].label){
		if(treap[root].left==INF)
			treap[root].left=current;
		else
			PushBinaryTree(current,treap[root].left);
	}
	else{
		if(treap[root].right==INF)
			treap[root].right=current;
		else
			PushBinaryTree(current,treap[root].right);
	}
}

void PushCartesianTree(int current,int destination){
	if(treap[current].priority<treap[destination].priority){
		treap[current].parent=destination;
		treap[destination].right=current;
	}
	else{
		treap[current].left=destination;
		if(treap[destination].parent!=INF)
			PushCartesianTree(current,treap[destination].parent);
		treap[destination].parent=current;
	}
}

void PrintTreap(int root){
	printf("(");
	if(treap[root].left!=INF)
		PrintTreap(treap[root].left);
	printf("%s/%d",treap[root].label.c_str(),treap[root].priority);
	if(treap[root].right!=INF)
		PrintTreap(treap[root].right);
	printf(")");
}

int main(){
	int treapSize;
	while(scanf("%d",&treapSize) && treapSize!=0){
		for(int i=0;i<treapSize;i++){
			char cbuffer[20];
			scanf("%s",cbuffer);
			for(int mid=0;cbuffer[mid]!='\0';mid++)
				if(cbuffer[mid]=='/'){
					cbuffer[mid]='\0';
					treap[i].label=string(cbuffer);
					treap[i].priority=atoi(cbuffer+mid+1);
					break;
				}
			treap[i].left=treap[i].right=treap[i].parent=INF;
		}
		sort(treap,treap+treapSize,CompareByLabel);
		for(i=1;i<treapSize;i++)
			PushCartesianTree(i,i-1);
		for(i=0;i<treapSize;i++)
			if(treap[i].parent==INF){
				PrintTreap(i);
				break;
			}
		printf("\n");
	}
	return 0;
}