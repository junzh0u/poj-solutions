#include<iostream.h>
#include<math.h>
#include<iomanip.h>
struct point// 一个点
{
  int x,y;
};

struct 
{
	point begin,end;
    int sign;
}edge[1000];//一条边

double calculate(point a,point b) //计算两个点之间的距离
{
	return (sqrt((a.x-b.x+0.0)*1.0*(a.x-b.x)+(a.y-b.y+0.0)*1.0*(a.y-b.y)));
}

double distance(int i,int last)//计算两条线之间的距离
{ 
	int erect,flat;
	double between;
    if(edge[i].sign==edge[last].sign)
	{
		if(edge[i].sign==1) //两条线段都是水平的
		{
			if(edge[i].end.x<edge[last].begin.x) between=calculate(edge[i].end,edge[last].begin);
			   else if(edge[i].begin.x>edge[last].end.x) between=calculate(edge[i].begin,edge[last].end);
				else between=fabs(edge[i].begin.y-edge[last].begin.y);
		}
		else //两条线段都是竖直的
		{
			if(edge[i].begin.y>edge[last].end.y) between=calculate(edge[i].begin,edge[last].end);
			   else if(edge[i].end.y<edge[last].begin.y) between=calculate(edge[i].end,edge[last].begin);
					else between=fabs(edge[i].begin.x-edge[last].begin.x);
		}
	}
	else 
	{
		 if(edge[i].sign==1) {erect=last;flat=i;}
		 else {erect=i;flat=last;} //erect 记录竖线，flat记录横线
		 if(edge[erect].end.x<edge[flat].begin.x) //图4，5，6中，竖线均在横线的左侧。
		 {
			if(edge[erect].begin.y>edge[flat].begin.y)
				between=calculate(edge[erect].begin,edge[flat].begin);
			else if(edge[erect].end.y<edge[flat].begin.y)
				between=calculate(edge[erect].end,edge[flat].begin);
			else between=fabs(edge[erect].begin.x-edge[flat].begin.x);
		}
		else if(edge[erect].begin.x>edge[flat].end.x)//图7，8，9中，竖线均在横线的右侧
		{
			if(edge[erect].begin.y>edge[flat].begin.y)
				between=calculate(edge[erect].begin,edge[flat].end);
			else if(edge[erect].end.y<edge[flat].begin.y)
				between=calculate(edge[erect].end,edge[flat].end);
			else between=fabs(edge[erect].begin.x-edge[flat].end.x);
		}
		else //图10，图11，图12中，竖线的x坐标介于横线两端的x坐标之间
		{ 
			if(edge[erect].begin.y>edge[flat].begin.y)
				between=fabs(edge[erect].begin.y-edge[flat].begin.y);
			else if(edge[erect].end.y<edge[flat].begin.y)
				between=fabs(edge[erect].end.y-edge[flat].begin.y);
			else between=0;
		}
	}
    return between;
}
void main()
{
	int i,n,l,min,used[1000],last;
	double between,weight[1000];
	cin>>n;
	while(n>0)
	{ 
		for(i=0;i<n;i++)//初始化，记录各条边
		{
			cin>>edge[i].begin.x>>edge[i].begin.y>>l;
			if(l>0)
			{
				edge[i].end.x=edge[i].begin.x+l;
                edge[i].end.y=edge[i].begin.y; 
				edge[i].sign=1;//1 表示weihengxian
			}
			else 
			{
				edge[i].end.x=edge[i].begin.x;
				edge[i].end.y=edge[i].begin.y-l;
				edge[i].sign=-1;// -1 表示为竖线
			}
		}
		for(i=0;i<n;i++)
		{   
			weight[i]=9999999;
   	        used[i]=0;// 0 表示未被标记过
		}
		last=0;
	    used[0]=1;//标记起始点已被选过
		weight[0]=0;//起始线所代表的点权值为0
	    while(last!=1)//只要last不为目标点
		{
			
			for(i=0;i<n;i++)
				if(used[i]==0)//如果i未被标记过
				{   
				    between=distance(i,last);//求i到last的距离
				    if(weight[last]>between) between=weight[last];
				    if((weight[i]-between)>1e-10) weight[i]=between;
				}
	       	   
				//从未被标记的点中选出权值最小的点
				i=0;
				while(used[i]==1) i++;
				min=i;
				for(i;i<n;i++)
					if((used[i]==0)&&((weight[min]-weight[i])>1e-10))
			              min=i;
                last=min;
				used[min]=1;
		}
		cout<<setiosflags(ios::fixed)<<setprecision(2)<<weight[1]<<endl;
		cin>>n;
	}
}