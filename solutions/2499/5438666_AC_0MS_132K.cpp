#include<cstdio>

int main(){
	int scenarioNum;
	scanf("%d",&scenarioNum);
	for(int scenarioID=1;scenarioID<=scenarioNum;scenarioID++){
		int a,b;
		scanf("%d%d",&a,&b);
		int left=0,right=0;
		while(a!=1 || b!=1){
			if(a>b){
				int count=a/b;
				if(a%b==0)
					count--;
				left+=count;
				a-=count*b;
			}
			else{
				int count=b/a;
				if(b%a==0)
					count--;
				right+=count;
				b-=count*a;
			}
		}
		printf("Scenario #%d:\n%d %d\n\n",scenarioID,left,right);
	}
	return 0;
}