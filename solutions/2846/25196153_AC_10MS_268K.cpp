// POJ 2846 - Kalii
// Model: gpt-5.6-terra
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
bool leap(int y){return y%400==0||(y%4==0&&y%100!=0);}
int days(int m,int y){int a[]={0,31,28,31,30,31,30,31,31,30,31,30,31};return a[m]+(m==2&&leap(y));}
int serial(int m,int d,int y){int s=d; for(int i=1;i<m;++i)s+=days(i,y); for(int i=1;i<y;++i)s+=365+leap(i); return s;}
int main(){int n,M,D,Y,m,d;cin>>n;for(int z=1;z<=n;++z){scanf("%d/%d/%d %d/%d",&M,&D,&Y,&m,&d);int t=serial(M,D,Y), by=Y, bd=1000000;for(int y=Y-1;y<=Y+1;++y){int x=serial(m,d,y)-t;if(abs(x)<abs(bd)){bd=x;by=y;}}cout<<z<<' ';if(bd==0)cout<<"SAME DAY\n";else if(abs(bd)>7)cout<<"OUT OF RANGE\n";else cout<<m<<'/'<<d<<'/'<<by<<" IS "<<abs(bd)<<" DAY"<<(abs(bd)==1?"":"S")<<(bd<0?" PRIOR\n":" AFTER\n");}}
