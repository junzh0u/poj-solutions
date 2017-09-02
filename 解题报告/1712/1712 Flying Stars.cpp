#include<iostream.h>
#include<string.h>

//所有时间是格林尼治时间
//所有时间以分钟为单位

struct station{
	char sn[6];		//飞机编号
	int tag;		//目的地
	int leave;		//起飞时间
	int run;		//运行时间
} st[110][310];		//储存各个航班的信息

char name[110][30];		//地名
char time[10];			
int delay[110];		//各机场延时
int timedifference[110];	//各机场时差
int airnum[110];	//各机场航班数
int starttime,namenum,n,m;
bool use[110];		//
int queue[110];		//队中每个机场的编号
int quenum;			//队中机场数
int quetime[110];		//队中到每个机场的时间 [0,24*60)
int sumtime[110];		//从起始机场到队中各机场的最短时间
int father[110];		//到队中各机场的前一个机场编号
char quesn[110][6];		//最早到队中各机场的飞机编号
char temsn[110][6];		//没设备那么太大用，把数组的顺序调换一下

void main()
{
	int i,j,k,p,pt,min,x,y,z;
	char place[30];
	char c;

	//超大规模的输入信息：
	cin>>name[0]>>name[1];
	cin>>time;
	starttime=(time[0]-'0')*600+(time[1]-'0')*60+(time[3]-'0')*10+time[4]-'0';
	cin>>n;
	namenum=2;
	for (i=0;i<n;i++)
	{
		cin>>place;
		for (k=0;k<namenum;k++)
			if (strcmp(place,name[k])==0)
			{
				p=k;
				break;
			}
		if (k>=namenum) 
		{
			p=namenum;
			strcpy(name[namenum],place);
			namenum++;
		}
		cin>>c>>time;
		if (c=='+')
			timedifference[p]=(time[0]-'0')*600+(time[1]-'0')*60+(time[3]-'0')*10+time[4]-'0';
		else
			timedifference[p]=0-((time[0]-'0')*600+(time[1]-'0')*60+(time[3]-'0')*10+time[4]-'0');
		cin>>time;
		delay[p]=(time[0]-'0')*600+(time[1]-'0')*60+(time[3]-'0')*10+time[4]-'0';
		cin>>m;
		airnum[p]=m;
		for (j=0;j<m;j++)
		{
			cin>>st[p][j].sn;
			cin>>place;
			for (k=0;k<namenum;k++)
				if (strcmp(place,name[k])==0)
				{
					st[p][j].tag=k;
					break;
				}
			if (k>=namenum) 
			{
				st[p][j].tag=namenum;
				strcpy(name[namenum],place);
				namenum++;
			}
			cin>>time;
			st[p][j].leave=(time[0]-'0')*600+(time[1]-'0')*60+(time[3]-'0')*10+time[4]-'0';
			st[p][j].leave-=timedifference[p];		//换成格林尼治时间
			while (st[p][j].leave<0) st[p][j].leave+=24*60;		//保证在[0,24*60)范围内	
			while (st[p][j].leave>=24*60) st[p][j].leave-=24*60;
			cin>>time;
			st[p][j].run=(time[0]-'0')*600+(time[1]-'0')*60+(time[3]-'0')*10+time[4]-'0';
		}
	}
	//终于输完了!!!  :~(

	//未加任何变动的dijkstra求最短路算法
	quenum=1;
	queue[0]=0;
	use[0]=true;
	quetime[0]=starttime-timedifference[0];
	while (quetime[0]<0) quetime[0]+=24*60;
	while (quetime[0]>=24*60) quetime[0]-=24*60;
	sumtime[0]=0;
	father[0]=-1;
	while(1)
	{
		//找从队中机场到非队中机场的最早时间
		min=2147483647;
		for (i=0;i<quenum;i++)
		{
			for (j=0;j<airnum[queue[i]];j++)
			{
				if (use[st[queue[i]][j].tag]==true) continue;
				if (quetime[i]+delay[queue[i]]<=st[queue[i]][j].leave)
					pt=sumtime[i]+st[queue[i]][j].leave-quetime[i]+st[queue[i]][j].run;
				else
					pt=sumtime[i]+st[queue[i]][j].leave-quetime[i]+st[queue[i]][j].run+24*60;
				if (pt<min)
				{
					min=pt;
					x=queue[i];
					y=j;
					z=i;
				}
			}
		}
		//将该机场放入队中
		queue[quenum]=st[x][y].tag;
		quetime[quenum]=st[x][y].leave+st[x][y].run;
		while (quetime[quenum]<0) quetime[quenum]+=60*24;
		while (quetime[quenum]>=24*60) quetime[quenum]-=24*60;
		sumtime[quenum]=min;
		strcpy(quesn[quenum],st[x][y].sn);
		father[quenum]=z;
		quenum++;
		use[st[x][y].tag]=true;
		if (st[x][y].tag==1) break;
	}

	//输出成的d:hh:mm格式(这种方法比较老土)
	quenum--;
	cout<<sumtime[quenum]/(24*60)<<":";
	sumtime[quenum]=sumtime[quenum]%(24*60);
	cout<<sumtime[quenum]/600;
	sumtime[quenum]=sumtime[quenum]%600;
	cout<<sumtime[quenum]/60<<":";
	sumtime[quenum]=sumtime[quenum]%60;
	cout<<sumtime[quenum]/10;
	sumtime[quenum]=sumtime[quenum]%10;
	cout<<sumtime[quenum]<<endl;
	
	quetime[quenum]+=timedifference[1];
	while (quetime[quenum]<0) quetime[quenum]+=60*24;
	while (quetime[quenum]>=24*60) quetime[quenum]-=24*60;
	cout<<quetime[quenum]/600;
	quetime[quenum]=quetime[quenum]%600;
	cout<<quetime[quenum]/60<<":";
	quetime[quenum]=quetime[quenum]%60;
	cout<<quetime[quenum]/10;
	quetime[quenum]=quetime[quenum]%10;
	cout<<quetime[quenum]<<endl;

	//依次输出班机编号
	j=0;
	for (i=quenum;father[i]!=-1;i=father[i])
	{	
		strcpy(temsn[j],quesn[i]);
		j++;
	}
	for (i=j-1;i>=0;i--)
		cout<<temsn[i]<<endl;
}