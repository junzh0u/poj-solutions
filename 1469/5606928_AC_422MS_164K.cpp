#include<cstdio>
#include<algorithm>
using namespace std;

const int COURSE_NUM_MAX=100,STUDENT_NUM_MAX=300,NO_COURSE=-1;

int courseNum,studentNum;
bool visit[COURSE_NUM_MAX][STUDENT_NUM_MAX],studentTransvered[STUDENT_NUM_MAX];
int represent[STUDENT_NUM_MAX];

bool findAndFlipAugumentPath(int courseID){
	for(int studentID=0;studentID<studentNum;studentID++)
		if(!studentTransvered[studentID] && visit[courseID][studentID]){
			studentTransvered[studentID]=true;
			if(represent[studentID]==NO_COURSE || findAndFlipAugumentPath(represent[studentID])){
				represent[studentID]=courseID;
				return true;
			}
		}
	return false;
}

bool hungary(){
	if(studentNum<courseNum)
		return false;
	fill(represent,represent+studentNum,NO_COURSE);
	for(int courseID=0;courseID<courseNum;courseID++){
		fill(studentTransvered,studentTransvered+studentNum,false);
		if(!findAndFlipAugumentPath(courseID))
			return false;
	}
	return true;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		scanf("%d%d",&courseNum,&studentNum);
		for(int courseID=0;courseID<courseNum;courseID++){
			fill(visit[courseID],visit[courseID]+studentNum,false);
			int studentVisitNum;
			scanf("%d",&studentVisitNum);
			while(studentVisitNum-->0){
				int studentID;
				scanf("%d",&studentID);
				studentID--;
				visit[courseID][studentID]=true;
			}
		}
		printf(hungary()?"YES\n":"NO\n");
	}
	return 0;
}