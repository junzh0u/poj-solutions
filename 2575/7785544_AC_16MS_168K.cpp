#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<algorithm>

const int SEQ_LEN_MAX=3000;

int sequenceLen;
//int sequence[SEQ_LEN_MAX];
bool diffExist[SEQ_LEN_MAX];//,diffNum;

int main(){
	while(scanf("%d",&sequenceLen)!=EOF){
		std::fill(diffExist,diffExist+sequenceLen,false);
		//diffNum=0;
		bool jolly=true;
		int prev,current;
		scanf("%d",&prev);
		for(int i=1;i<sequenceLen;i++){
			scanf("%d",&current);
			if(!jolly)
				continue;
			int diff=std::abs(current-prev);
			if(diff<1 || diff>sequenceLen-1 || diffExist[diff]){
				jolly=false;
				continue;
			}
			diffExist[diff]=true;
			prev=current;
			//diffNum++;
		}
		printf(jolly?"Jolly\n":"Not jolly\n");
	}
	return 0;
}