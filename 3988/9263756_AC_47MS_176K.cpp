#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int COURSE_NUM_MAX=300,INTERVAL=5,TIME_MAX=1000,NONE=-1;

struct Course{
	int begin,end;
	bool selected;
};

bool courseLess(const Course &left,const Course &right){
	if(left.begin!=right.begin)
		return left.begin<right.begin;
	return left.end<right.end;
}

int main(){
	while(true){
		int courseNum;
		scanf("%d",&courseNum);
		if(!courseNum)
			return 0;
		Course courses[COURSE_NUM_MAX];
 		int timeMin=TIME_MAX,timeMax=0;
		for(int i=0;i<courseNum;i++){
			scanf("%d%d",&courses[i].begin,&courses[i].end);
 			if(timeMin>courses[i].begin)
 				timeMin=courses[i].begin;
 			if(timeMax<courses[i].end)
 				timeMax=courses[i].end;
		}
		std::sort(courses,courses+courseNum,courseLess);
		int selectedNumMax=0;
		for(int first=0;first<INTERVAL;first++){
			for(int i=0;i<courseNum;i++)
				courses[i].selected=false;
			int selectedNum=0;
			for(int time=timeMin+first;time<timeMax;time+=INTERVAL){
				int courseSelect=NONE;
				for(int i=0;i<courseNum;i++){
					if(courses[i].selected)
						continue;
					if(time<courses[i].begin)
						break;
					if(time<courses[i].end && (courseSelect==NONE || courses[i].end<courses[courseSelect].end))
						courseSelect=i;
				}
				if(courseSelect!=NONE){
					courses[courseSelect].selected=true;
					selectedNum++;
				}
			}
			if(selectedNumMax<selectedNum)
				selectedNumMax=selectedNum;
		}
		printf("%d\n",selectedNumMax);
	}
}