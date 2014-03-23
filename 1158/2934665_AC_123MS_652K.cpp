#include<cstdio>
#include<algorithm>
using namespace std;

const int BLUE=0,PURPLE=1,INF=2147483647;//INT_MAX;
const int MAX=310;
int juncSour,juncDest,juncNum,roadNum;
int color[MAX+1]={0},remain[MAX+1]={0},duration[MAX+1][2]={0},length[MAX+1][MAX+1]={0},pathLen[MAX+1]={0},road[2*MAX*MAX][2];

int min(int left,int right){
	return (left<right)?left:right;
}

int Color(int junction,int time){
	time+=duration[junction][0]+duration[junction][1]-remain[junction];
	if(color[junction]==BLUE)
		time-=duration[junction][1];
	time=time%(duration[junction][0]+duration[junction][1]);
	return (time<duration[junction][0])?BLUE:PURPLE;
}

int Remain(int junction,int time){
	time+=duration[junction][0]+duration[junction][1]-remain[junction];
	if(color[junction]==BLUE)
		time-=duration[junction][1];
	time=duration[junction][0]+duration[junction][1]-time%(duration[junction][0]+duration[junction][1]);
	if(time>duration[junction][1])
		time-=duration[junction][1];
	return time;
}

int ArriveTime(int from,int to,int beginTime){
	int fromColor=Color(from,beginTime),toColor=Color(to,beginTime);
	if(fromColor==toColor)
		return beginTime+length[from][to];
	int fromRemain=Remain(from,beginTime),toRemain=Remain(to,beginTime);
	if(fromRemain!=toRemain)
		return beginTime+length[from][to]+min(fromRemain,toRemain);
	int fromNext=duration[from][1-fromColor],toNext=duration[to][1-toColor];
	if(fromNext!=toNext)
		return beginTime+length[from][to]+fromRemain+min(fromNext,toNext);
	int fromNextNext=duration[from][fromColor],toNextNext=duration[to][toColor];
	if(fromNextNext!=toNextNext)
		return beginTime+length[from][to]+fromRemain+fromNext+min(fromNextNext,toNextNext);
	return INF;
}

int main(){
	scanf("%d%d%d%d",&juncSour,&juncDest,&juncNum,&roadNum);
	for(int i=1;i<=juncNum;i++){
		char temp[2];
		scanf("%s%d%d%d",temp,&remain[i],&duration[i][0],&duration[i][1]);
		color[i]=(temp[0]=='B')?BLUE:PURPLE;
		pathLen[i]=INF;
	}
	pathLen[juncSour]=0;
	for(i=0;i<roadNum;i++){
		int from,to,len;
		scanf("%d%d%d",&from,&to,&len);
		road[2*i][0]=from;
		road[2*i][1]=to;
		road[2*i+1][0]=to;
		road[2*i+1][1]=from;
		length[from][to]=length[to][from]=len;
	}
	bool anyChange=true;
	for(i=1;i<=juncNum & anyChange;i++){
		anyChange=false;
		for(int j=0;j<2*roadNum;j++){
			if(pathLen[road[j][0]]<INF){
				int time=ArriveTime(road[j][0],road[j][1],pathLen[road[j][0]]);
				if(pathLen[road[j][1]]>time){
					pathLen[road[j][1]]=time;
					anyChange=true;
				}
			}
		}
	}
	printf("%d\n",(pathLen[juncDest]==INF)?0:pathLen[juncDest]);
	return 0;
}