#include<cstdio>

int main(){
	int length;
	scanf("%d",&length);
	int numOfNine=0,last=-1;
	while(length-->0){
		int left,right;
		getchar();left=getchar()-'0';
		getchar();right=getchar()-'0';
		int sum=left+right;
		if(sum==9){
			numOfNine++;
		}
		else if(sum<9){
			if(last>=0)
				putchar(last+'0');
			while(numOfNine>0){
				putchar('9');
				numOfNine--;
			}
			last=sum;
		}
		else{
			printf("%d",last+1);
			while(numOfNine>0){
				putchar('0');
				numOfNine--;
			}
			last=sum%10;
		}
	}
	if(last>=0)
		putchar(last+'0');
	while(numOfNine>0){
		putchar('9');
		numOfNine--;
	}
	return 0;
}