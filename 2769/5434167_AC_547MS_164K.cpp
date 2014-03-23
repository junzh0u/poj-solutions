#include<cstdio>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int studentNum;
		scanf("%d",&studentNum);
		int *SIN=new int[studentNum];
		for(int i=0;i<studentNum;i++)
			scanf("%d",SIN+i);
		for(int modulus=studentNum;;modulus++){
			bool *used=new bool[modulus];
			for(int i=0;i<modulus;i++)
				used[i]=false;
			bool unique=true;
			for(int i=0;i<studentNum;i++){
				if(used[SIN[i]%modulus]){
					unique=false;
					break;
				}
				used[SIN[i]%modulus]=true;
			}
			delete[] used;
			if(unique){
				printf("%d\n",modulus);
				break;
			}
		}
		delete[] SIN;
	}
	return 0;
}