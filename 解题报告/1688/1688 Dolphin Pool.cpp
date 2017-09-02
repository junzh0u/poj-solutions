#include<iostream>
#include<iomanip>
#include<string>
#include<algorithm>
#include<math.h>
#include<assert.h>
using namespace std;
#define NEG 1e-7  //浮点数比较中的允许误差 

struct intersect{      //两圆交点 
    int other;         //记录同一交点在另一圆上的位置。other=另一圆序号*100+交点在圆上的序号。 
    char sign;         //相交弧与交点之间的相对位置（顺时针或逆时针） 
    double x, y, x0, y0;  //（x，y）是交点坐标，（x0，y0）是圆心坐标 
}p[20][40];
struct circle{
    int x,y,r;
}cir[20];          //圆 
int n,nsec[20];    //圆的个数和每个圆上的交点数目 
bool v[20][40];    //交点是否访问过 

/*下面三个函数是快速排序*/ 
bool operator<(const intersect& x, const intersect& y)
{
    return atan2(x.y-x.y0,x.x-x.x0)<atan2(y.y-y.y0,y.x-y.x0)-NEG;
}
int Partition(intersect b[],int l,int r,int a)
{
    intersect x=b[l],t;
    int i=l-1,j=r+1;
    while(1)
    {
        do{
            --j;
        }while(x<b[j]);
        do{
            ++i;
        }while(b[i]<x);
        if(i<j){
            p[b[i].other/100][b[i].other%100].other=a*100+j;  //排序中加入对other属性的维护 
            p[b[j].other/100][b[j].other%100].other=a*100+i;
            t=b[i];b[i]=b[j];b[j]=t;
        }
        else return j;
    }
    return l;
}
void quicksort(intersect b[],int l,int r,int a)
{
    int n;
    if(l<r)
    {
        n=Partition(b,l,r,a);
        quicksort(b,l,n,a);
        quicksort(b,n+1,r,a);
    }
    return;
}

/*求两圆交点*/ 
void GetPoint(int a,int b,intersect& p1,intersect& p2)
{
    int A,B,C,D,E,F;
    double A1,B1,C1;
    A=-2*cir[a].x;B=-2*cir[a].y;
    C=cir[a].x*cir[a].x+cir[a].y*cir[a].y-cir[a].r*cir[a].r;
    D=-2*cir[b].x;E=-2*cir[b].y;
    F=cir[b].x*cir[b].x+cir[b].y*cir[b].y-cir[b].r*cir[b].r;
    if(A==D)                           //交线平行于x轴 
    {
        p1.y=p2.y=(F-C)/(double)(B-E);
        p1.x=(-A+sqrt((double)A*A-4*(p1.y*p1.y+B*p1.y+C)))/2.0;
        p2.x=(-A-sqrt((double)A*A-4*(p2.y*p2.y+B*p2.y+C)))/2.0;
    }
    else if(B==E)                      //交线平行于y轴 
    {
        p1.x=p2.x=(F-C)/(double)(A-D);
        p1.y=(-B+sqrt((double)B*B-4*(p1.x*p1.x+A*p1.x+C)))/2.0;
        p2.y=(-B-sqrt((double)B*B-4*(p2.x*p2.x+A*p2.x+C)))/2.0;
    }
    else                               //一般情形 
    {
        A1=(double)(E-B)*(E-B)+(double)(A-D)*(A-D);
        B1=2.0*(E-B)*(F-C)+((double)D)*(A-D)*(E-B)+(A-D)*(A-D)*(double)E;
        C1=(double)(F-C)*(F-C)+((double)D)*(A-D)*(F-C)+((double)F)*(A-D)*(A-D);
        p1.y=(-B1+sqrt(B1*B1-4*A1*C1))/(2*A1);
        p2.y=(-B1-sqrt(B1*B1-4*A1*C1))/(2*A1);
        p1.x=(p1.y*(E-B)+(F-C))/(A-D);
        p2.x=(p2.y*(E-B)+(F-C))/(A-D);
    }
    return;
}
/*判断焦点p0是否被圆k覆盖*/ 
bool Cover(int k,intersect& p0)
{
    if((p0.x-cir[k].x)*(p0.x-cir[k].x)+(p0.y-cir[k].y)*(p0.y-cir[k].y)>cir[k].r*cir[k].r+NEG)
        return false;
    else
        return true;
}
/*依序访问交点，寻找小区域*/ 
int Visit(int i,int j)
{
    double lx,ly,s;
    s=0;lx=p[i][j].x;ly=p[i][j].y;
    while(!v[i][j])
    {
        v[i][j]=true;
        i=p[i][j].other;
        j=i%100;i/=100;           //找下一个圆 
        v[i][j]=true;
        if(j<nsec[i]-1)j++;       //找下一个交点 
        else j=0;
        s+=(lx*p[i][j].y-ly*p[i][j].x)/2;  //计算有向面积 
        lx=p[i][j].x;ly=p[i][j].y;
    }                             //循环y结束，找到一个由圆弧构成的环 
    if(s>-NEG)return 0;           //有向面积为正，该区域边界是由多个圆组成的连通区域的外边界 
    else return 1;                //有向区域为负，该区域是由多个圆围成的小区域。 
}
int main()
{
    int t,index,i,j,k;
    intersect p1,p2;
    cin>>t;
    for(index=1;index<=t;index++)
    {
    /*读入数据*/ 
        cin>>n;
        for(i=0;i<n;i++)
            cin>>cir[i].x>>cir[i].y>>cir[i].r;
        fill(nsec,nsec+n,0);
        
        /*求交点*/ 
        for(i=0;i<n;i++)
        {
            for(j=i+1;j<n;j++)
                if((cir[i].x-cir[j].x)*(cir[i].x-cir[j].x)+(cir[i].y-cir[j].y)*(cir[i].y-cir[j].y)<(cir[i].r+cir[j].r)*(cir[i].r+cir[j].r))
                {
                    GetPoint(i,j,p1,p2);            //求两圆交点 
                    for(k=0;k<n;k++)
                        if(k!=i&&k!=j&&Cover(k,p1))break;     //判断交点是否被覆盖
                    if(k==n)
                    {
                        p[i][nsec[i]].x=p1.x;p[i][nsec[i]].y=p1.y;
                        p[j][nsec[j]].x=p1.x;p[j][nsec[j]].y=p1.y;
                        p[i][nsec[i]].x0=cir[i].x;p[i][nsec[i]].y0=cir[i].y;
                        p[j][nsec[j]].x0=cir[j].x;p[j][nsec[j]].y0=cir[j].y;
                        p[i][nsec[i]].other=j*100+nsec[j];
                        p[j][nsec[j]].other=i*100+nsec[i];
                        //根据两交点对应半径的叉积判断相交弧在交点的顺/逆时针方向 
                        if((p1.x-cir[i].x)*(p2.y-cir[i].y)-(p1.y-cir[i].y)*(p2.x-cir[i].x)<-NEG)  
                        {
                            p[i][nsec[i]].sign='+';p[j][nsec[j]].sign='-';
                        }
                        else
                        {
                            p[i][nsec[i]].sign='-';p[j][nsec[j]].sign='+';
                        }
                        nsec[i]++;nsec[j]++;
                    }
                    for(k=0;k<n;k++)      //对另一个交点操作，同上 
                        if(k!=i&&k!=j&&Cover(k,p2))break;
                    if(k==n)
                    {
                        p[i][nsec[i]].x=p2.x;p[i][nsec[i]].y=p2.y;
                        p[j][nsec[j]].x=p2.x;p[j][nsec[j]].y=p2.y;
                        p[i][nsec[i]].x0=cir[i].x;p[i][nsec[i]].y0=cir[i].y;
                        p[j][nsec[j]].x0=cir[j].x;p[j][nsec[j]].y0=cir[j].y;
                        p[i][nsec[i]].other=j*100+nsec[j];
                        p[j][nsec[j]].other=i*100+nsec[i];
                        if((p1.x-cir[i].x)*(p2.y-cir[i].y)-(p1.y-cir[i].y)*(p2.x-cir[i].x)<-NEG)
                        {
                            p[i][nsec[i]].sign='-';p[j][nsec[j]].sign='+';
                        }
                        else
                        {
                            p[i][nsec[i]].sign='+';p[j][nsec[j]].sign='-';
                        }
                        nsec[i]++;nsec[j]++;
                    }
                }
            quicksort(p[i],0,nsec[i]-1,i);
        }
        fill(&v[0][0],&v[n][0],false);
        int s=0;
        /*从每个未访问的交点出发，寻找包含该点的区域*/
        for(i=0;i<n;i++)
            for(j=0;j<nsec[i];j++)
                if(!v[i][j])   
                    if(p[i][j].sign=='-')s+=Visit(i,j);
                    else s+=Visit(p[i][j].other/100,p[i][j].other%100);
        cout<<s<<endl;
    }
    return 0;
}

