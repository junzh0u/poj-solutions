#include<cstdio>
#include<algorithm>

int main(){
	int numberMax,digit;
	while(scanf("%d%d",&numberMax,&digit)!=EOF){
		int count=0;
		for(int number=2;number<=numberMax;number++){
			bool *exist=new bool[number];
			std::fill(exist,exist+number,false);
			for(int remainder=10;remainder>0 && !exist[remainder/10];remainder=(remainder%number)*10){
				exist[remainder/10]=true;
				if(remainder/number==digit)
					count++;
			}
			delete[] exist;
		}
		printf("%d\n",count);
	}
	return 0;
}