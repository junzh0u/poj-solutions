#include<cstdio>

int main(){
	int n;
	while(scanf("%d",&n)!=EOF){
		for(int i=2,remainder=1;;i++){
			remainder=(remainder*10+1)%n;
			if(remainder==0){
				printf("%d\n",i);
				break;
			}
		}
	}
	return 0;
}