#include<cstdio>

int Root(int id,int *father){
	if(id==-1 || father[id]==id)
		return id;
	return father[id]=Root(father[id],father);
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int criminalNum,messageNum;
		scanf("%d%d",&criminalNum,&messageNum);
		int *father=new int[criminalNum],*oppsite=new int[criminalNum];
		for(int i=0;i<criminalNum;i++){
			father[i]=i;
			oppsite[i]=-1;
		}
		while(messageNum-->0){
			char command;
			int left,right;
			scanf(" %c%d%d",&command,&left,&right);
			left=Root(left-1,father);
			right=Root(right-1,father);
			if(command=='D'){
				if(oppsite[left]!=-1)
					father[right]=Root(oppsite[left],father);
				if(oppsite[right]!=-1)
					father[left]=Root(oppsite[right],father);
				oppsite[left]=right;
				oppsite[right]=left;
			}
			else{
				if(left==right)
					printf("In the same gang.\n");
				else if(left==Root(oppsite[right],father))
					printf("In different gangs.\n");
				else
					printf("Not sure yet.\n");
			}
		}
		delete[] father;
		delete[] oppsite;
	}
	return 0;
}