#include<cstdio>
#include<string>
using namespace std;

void PrintPostorder(char *preorder,int beginPre,char* inorder,int beginIn,int size){
	if(size==0)
		return;
	if(size==1){
		printf("%c",preorder[beginPre]);
		return;
	}
	for(int leftSize=0;leftSize<size;leftSize++)
		if(inorder[beginIn+leftSize]==preorder[beginPre]){
			PrintPostorder(preorder,beginPre+1,inorder,beginIn,leftSize);
			PrintPostorder(preorder,beginPre+1+leftSize,inorder,beginIn+1+leftSize,size-leftSize-1);
			printf("%c",preorder[beginPre]);
			return;
		}
}

int main(){
	const int LENGTH_MAX=26;
	char preorder[LENGTH_MAX+1],inorder[LENGTH_MAX+1];
	while(scanf("%s%s",preorder,inorder)!=EOF){
		PrintPostorder(preorder,0,inorder,0,strlen(preorder));
		printf("\n");
	}
	return 0;
}