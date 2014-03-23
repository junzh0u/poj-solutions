#include<cstdio>
#include<algorithm>
using namespace std;

const int DIRECTION_NUM=8;
const int ROW_STEP[DIRECTION_NUM]={-2,-2,-1,-1,1,1,2,2},COL_STEP[DIRECTION_NUM]={-1,1,-2,2,-2,2,-1,1};

bool Travel(const int row,const int col,const int height,const int width,bool **visited,const int visitCount,int **direction){
	if(visitCount==height*width)
		return true;
	for(int next=0;next<DIRECTION_NUM;next++){
		int rowNext=row+ROW_STEP[next],colNext=col+COL_STEP[next];
		if(rowNext>=0 && rowNext<height && colNext>=0 && colNext<width && !visited[rowNext][colNext]){
			visited[rowNext][colNext]=true;
			bool temp=Travel(rowNext,colNext,height,width,visited,visitCount+1,direction);
			visited[rowNext][colNext]=false;
			if(temp){
				direction[row][col]=next;
				return true;
			}
		}
	}
	return false;
}

int main(){
	int scenarioNum;
	scanf("%d",&scenarioNum);
	for(int scenarioID=1;scenarioID<=scenarioNum;scenarioID++){
		int width,height;
		scanf("%d%d",&width,&height);
		bool **visited=new bool*[height];
		int **direction=new int*[height];
		for(int row=0;row<height;row++){
			visited[row]=new bool[width];
			direction[row]=new int[width];
			fill(visited[row],visited[row]+width,false);
		}
		printf("Scenario #%d:\n",scenarioID);
		visited[0][0]=true;
		if(!Travel(0,0,height,width,visited,1,direction))
			printf("impossible");
		else{
			int row=0,col=0,count=0;
			while(true){
				printf("%c%d",row+'A',col+1);
				count++;
				if(count>=height*width)
					break;
				int next=direction[row][col];
				row+=ROW_STEP[next];
				col+=COL_STEP[next];
			}
		}
		printf("\n\n");
		for(int row=0;row<height;row++){
			delete[] visited[row];
			delete[] direction[row];
		}
		delete[] visited;
		delete[] direction;
	}
	return 0;
}