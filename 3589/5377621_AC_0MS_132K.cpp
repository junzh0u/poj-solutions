#include<cstdio>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		char mind[5],guess[5];
		scanf("%s%s",mind,guess);
		int A=0,B=0;
		for(int i=0;i<4;i++){
			if(guess[i]==mind[i])
				A++;
			else
				for(int j=0;j<4;j++)
					if(j!=i && guess[i]==mind[j]){
						B++;
						break;
					}
		}
		printf("%dA%dB\n",A,B);
	}
	return 0;
}