#include <stdio.h>
#include <memory.h>
#include <math.h>
const int dir[8][2]={
	{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}
};

bool map[51][51][4];
int m,n;

int Direct(int x1,int y1,int x2,int y2)
{
	int xa,ya,d;
	xa=x2-x1,ya=y2-y1;
	if(xa!=0)xa/=abs(xa);
	if(ya!=0)ya/=abs(ya);
	for(d=0;d<8;d++)
	{
		if(dir[d][0]==xa && dir[d][1]==ya)
			return d;
	}
	return -1;
}

int main()
{
	int i,j,k,x1,y1,x2,y2,d;
	int sum[9];
	scanf("%d %d %d",&m,&n,&k);
	memset(map,0,sizeof(map));
	for(i=1;i<=k;i++)
	{
		scanf("%d %d %d %d",&x1,&y1,&x2,&y2);
		d=Direct(x1,y1,x2,y2);
		while(x1!=x2 || y1!=y2)
		{
			map[x1][y1][d%4]=true;
			x1+=dir[d][0],y1+=dir[d][1];
		}
		map[x1][y1][d%4]=true;
	}
	for(i=0;i<=m;i++)
	{
		map[i][0][0]=map[i][n][0]=true;
	}
	for(i=0;i<=n;i++)
	{
		map[0][i][2]=map[m][i][2]=true;
	}
	for(i=0;i<=8;i++)
		sum[i]=0;
	for(i=0;i<=m;i++)
	{
		for(j=0;j<=n;j++)
		{
			//第一类三角形.
			if(map[i][j][3] && map[i][j][2])
			{
				x1=i-1,y1=j-1;
				x2=i,y2=j-1;
				while(x1>=0 && y1>=0)
				{
					if(map[x1][y1][0] && !map[x2][y2][1] && !map[x1][y1+1][1])
					{
						sum[1]++;
						break;
					}
					if(map[x1][y1][1] || map[x1][y1][2] || map[x2][y2][3] || map[x2][y2][1] || map[x1][y1+1][1])
						break;
					x1--,y1--,y2--;
				}
			}
			//第二类三角形
			if(map[i][j][2] && map[i][j][1])
			{
				x1=i+1,y1=j-1;
				x2=i,y2=j-1;
				while(x1<=m && y1>=0)
				{
					if(map[x1][y1][0] && !map[x2][y2][3] && !map[x1][y1+1][3])
					{
						sum[2]++;
						break;
					}
					if(map[x1][y1][3] || map[x1][y1][2] || map[x2][y2][1] || map[x2][y2][3] || map[x1][y1+1][3])
						break;
					x1++,y1--,y2--;
				}
			}
			//第三类三角形
			if(map[i][j][0] && map[i][j][1])
			{
				x1=i+1,y1=j-1;
				x2=i+1,y2=j;
				while(x1<=m && y1>=0)
				{
					if(map[x1][y1][2] && !map[x2][y2][3] && !map[x1-1][y1][3])
					{
						//printf("i=%d j=%d x1=%d y1=%d\n",i,j,x1,y1);
						sum[3]++;
						break;
					}
					if(map[x1][y1][3] || map[x1][y1][0] || map[x2][y2][1] || map[x2][y2][3] || map[x1-1][y1][3])
						break;
					x1++,y1--,x2++;
				}
			}
			//第四类三角形
			if(map[i][j][0] && map[i][j][3])
			{
				x1=i-1,y1=j-1;
				x2=i-1,y2=j;
				while(x1>=0 && y1>=0)
				{
					if(map[x1][y1][2] && !map[x2][y2][1] && !map[x1+1][y1][1])
					{
						sum[4]++;
						break;
					}
					if(map[x1][y1][1] || map[x1][y1][0] || map[x2][y2][3] || map[x2][y2][1] || map[x1+1][y1][1])
						break;
					x1--,y1--,x2--;
				}
			}
			//第五类三角形
			if(map[i][j][3] && map[i][j][2])
			{
				x1=i,y1=j-1;
				while(y1>=0 && j-y1+1<=2*i)
				{
					if(map[x1][y1][1])
					{
						sum[5]++;
						break;
					}
					if(map[x1][y1][0] || map[x1][y1][3])
						break;
					y1--;
					if(y1<0)
						break;
					if(map[x1][y1][1])
					{
						sum[5]++;
						break;
					}
					x2=i-(j-y1)/2;
					y2=(j+y1)/2;
					if(map[x1][y1][0] || map[x1][y1][3] || map[x2][y2][2])
						break;
					y1--;
				}
			}
			//第六类三角形
			if(map[i][j][0] && map[i][j][3])
			{
				x1=i+1,y1=j;
				while(x1<=m && x1-i+1<=2*(n-j))
				{
					if(map[x1][y1][1])
					{
						sum[6]++;
						break;
					}
					if(map[x1][y1][2] || map[x1][y1][3])
						break;
					x1++;
					if(x1>m)
						break;
					if(map[x1][y1][1])
					{
						sum[6]++;
						break;
					}
					x2=(x1+i)/2;
					y2=j+(x1-i)/2;
					if(map[x1][y1][2] || map[x1][y1][3] || map[x2][y2][0])
						break;
					x1++;
				}
			}
			//第七类三角形
			if(map[i][j][2] && map[i][j][1])
			{
				x1=i,y1=j-1;
				while(y1>=0 && j-y1+1<=2*(m-i))
				{
					if(map[x1][y1][3])
					{
						sum[7]++;
						break;
					}
					if(map[x1][y1][0] || map[x1][y1][1])
						break;
					y1--;
					if(y1<0)
						break;
					if(map[x1][y1][3])
					{
						sum[7]++;
						break;
					}
					x2=x1+(j-y1)/2;
					y2=(j+y1)/2;
					if(map[x1][y1][0] || map[x1][y1][1] || map[x2][y2][2])
						break;
					y1--;
				}
			}
			//第八类三角形
			if(map[i][j][0] && map[i][j][1])
			{
				x1=i+1,y1=j;
				while(x1<=m && x1-i+1<=2*j)
				{
					if(map[x1][y1][3])
					{
						sum[8]++;
						break;
					}
					if(map[x1][y1][2] || map[x1][y1][1])
						break;
					x1++;
					if(x1>m)
						break;
					if(map[x1][y1][3])
					{
						sum[8]++;
						break;
					}
					x2=(i+x1)/2;
					y2=j-(x1-i)/2;
					if(map[x1][y1][2] || map[x1][y1][1] || map[x2][y2][0])
						break;
					x1++;
				}
			}

		}
	}
	for(i=1;i<=8;i++)
	{
		sum[0]+=sum[i];
	}
	printf("%d\n",sum[0]);
	return 0;
}

