#include<cstdio>
#include<algorithm>

int Root(int index,int *father){
	if(index==father[index])
		return index;
	return father[index]=Root(father[index],father);
}

void Union(int left,int right,int *father){
	father[Root(left,father)]=Root(right,father);
}

int main(){
	int scenarioNum;
	scanf("%d",&scenarioNum);
	for(int scenarioID=1;scenarioID<=scenarioNum;scenarioID++){
		int bugNum,interactionNum;
		scanf("%d%d",&bugNum,&interactionNum);
		int *father=new int[bugNum],*oppsite=new int[bugNum];
		for(int i=0;i<bugNum;i++){
			father[i]=i;
			oppsite[i]=-1;
		}
		bool suspicious=false;
		while(interactionNum-->0){
			int left,right;
			scanf("%d%d",&left,&right);
			left--;right--;
			if(!suspicious){
				if(Root(left,father)==Root(right,father))
					suspicious=true;
				else{
					if(oppsite[left]!=-1)
						Union(oppsite[left],right,father);
					if(oppsite[right]!=-1)
						Union(left,oppsite[right],father);
					oppsite[left]=right;
					oppsite[right]=left;
				}
			}
		}
		printf("Scenario #%d:\n",scenarioID);
		if(suspicious)
			printf("Suspicious bugs found!\n\n");
		else
			printf("No suspicious bugs found!\n\n");
		delete[] oppsite;
		delete[] father;
	}
	return 0;
}