#include<cstdio>

int main(){
	while(true){
		int stoneNum,removeCode,stoneBegin;
		scanf("%d%d%d",&stoneNum,&removeCode,&stoneBegin);
		if(stoneNum==0 && removeCode==0 && stoneBegin==0)
			return 0;
		int stoneLeft=0;
		for(int stoneRemainNum=2;stoneRemainNum<=stoneNum;stoneRemainNum++){
			int insideShift=(removeCode-1)%(stoneRemainNum-1);
			stoneLeft=((insideShift+stoneLeft)%(stoneRemainNum-1)+1)%stoneRemainNum;
		}
		stoneLeft=(stoneLeft+stoneBegin-1)%stoneNum+1;
		printf("%d\n",stoneLeft);
	}
}