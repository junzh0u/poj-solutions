#include<cstdio>

int GetRoot(const int id,int *father){
	if(father[id]==id)
		return id;
	return father[id]=GetRoot(father[id],father);
}

int main(){
	while(true){
		int studentNum,groupNum;
		scanf("%d%d",&studentNum,&groupNum);
		if(studentNum==0 && groupNum==0)
			break;
		int *father=new int[studentNum];
		for(int i=0;i<studentNum;i++)
			father[i]=i;
		while(groupNum-->0){
			int groupSize,chief;
			scanf("%d%d",&groupSize,&chief);
			chief=GetRoot(chief,father);
			while(groupSize-->1){
				int member;
				scanf("%d",&member);
				father[GetRoot(member,father)]=chief;
			}
		}
		int count=0,suspectRoot=GetRoot(0,father);
		for(int i=0;i<studentNum;i++)
			if(GetRoot(i,father)==suspectRoot)
				count++;
		printf("%d\n",count);
		delete[] father;
	}
	return 0;
}