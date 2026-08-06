#include<cstdio>

int main(){
	int total,choose;
	while(scanf("%d%d",&total,&choose) && total!=0){
		if(choose>total-choose)
			choose=total-choose;
		double result=1;
		for(int i=0;i<choose;i++){
			//result/=i+1;
			result*=double(total-i)/(i+1);
		}
		printf("%.0lf\n",result);
	}
	return 0;
}