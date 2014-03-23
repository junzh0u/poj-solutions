#include<cstdio>

int GetRoot(int ID,int *father){
	if(father[ID]==ID)
		return ID;
	return father[ID]=GetRoot(father[ID],father);
}

int main(){
	const int STUDENT_MAX=50000;
	int studentNum,pairNum,caseID=1;
	int father[STUDENT_MAX];
	bool religionRoot[STUDENT_MAX];
	while(true){
		scanf("%d%d",&studentNum,&pairNum);
		if(studentNum==0 && pairNum==0)
			break;
		//int *father=new int[studentNum];
		for(int i=0;i<studentNum;i++)
			father[i]=i;
		while(pairNum-->0){
			int left,right;
			scanf("%d%d",&left,&right);
			left=GetRoot(left-1,father);
			right=GetRoot(right-1,father);
			if(left!=right)
				father[right]=left;
		}
		//bool *religionRoot=new bool[studentNum];
		for(int i=0;i<studentNum;i++)
			religionRoot[i]=false;
		for(int i=0;i<studentNum;i++)
			religionRoot[GetRoot(i,father)]=true;
		int religionCount=0;
		for(int i=0;i<studentNum;i++)
			if(religionRoot[i])
				religionCount++;
		printf("Case %d: %d\n",caseID++,religionCount);
		//delete[] religionRoot;
		//delete[] father;
	}
	return 0;
}