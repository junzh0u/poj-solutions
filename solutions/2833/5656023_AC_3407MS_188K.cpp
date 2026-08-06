#include<cstdio>
#include<queue>
#include<functional>

int main(){
	while(true){
		int greatestNum,leastNum,totalNum;
		scanf("%d%d%d",&greatestNum,&leastNum,&totalNum);
		if(greatestNum==0 && leastNum==0 && totalNum==0)
			break;
		std::priority_queue<int,std::vector<int>,std::greater<int>> greatest;
		std::priority_queue<int> least;
		double gradeAverage=0.0;
		for(int i=0;i<totalNum;i++){
			int grade;
			scanf("%d",&grade);
			gradeAverage+=grade;
			greatest.push(grade);
			if(i>=greatestNum)
				greatest.pop();
			least.push(grade);
			if(i>=leastNum)
				least.pop();
		}
		while(!greatest.empty()){
			gradeAverage-=greatest.top();
			greatest.pop();
		}
		while(!least.empty()){
			gradeAverage-=least.top();
			least.pop();
		}
		gradeAverage/=totalNum-greatestNum-leastNum;
		printf("%.6f\n",gradeAverage);
	}
	return 0;
}