#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int TREE_NUM_MAX=500;

struct Position{
	int x,y;
};

int main(){
	while(true){
		int treeNum;
		scanf("%d",&treeNum);
		if(!treeNum)
			return 0;

		int fieldWidth,fieldHeight;
		scanf("%d%d",&fieldWidth,&fieldHeight);

		Position trees[TREE_NUM_MAX];
		for(int i=0;i<treeNum;i++){
			scanf("%d%d",&trees[i].x,&trees[i].y);
			trees[i].x--;
			trees[i].y--;
		}

		int rectWidth,rectHeight;
		scanf("%d%d",&rectWidth,&rectHeight);

		int treeIncludeMax=0;
		for(int rectX=0;rectX+rectWidth<=fieldWidth;rectX++){
			for(int rectY=0;rectY+rectHeight<=fieldHeight;rectY++){
				int treeInclude=0;
				for(int treeIndex=0;treeIndex<treeNum;treeIndex++)
					if(	trees[treeIndex].x>=rectX && trees[treeIndex].x<rectX+rectWidth &&
						trees[treeIndex].y>=rectY && trees[treeIndex].y<rectY+rectHeight)
						treeInclude++;
				if(treeIncludeMax<treeInclude)
					treeIncludeMax=treeInclude;
			}
		}

		printf("%d\n",treeIncludeMax);
	}
}