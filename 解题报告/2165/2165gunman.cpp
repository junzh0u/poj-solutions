#include<stdio.h>
#include <math.h>
#define MIN 1e-10
int n;
char cont='t';/*记录是否有解*/
double xleft[101],xright[101],ylow[101],yhigh[101],z[101];/*记录n个窗口的坐标*/
double kyhigh,kylow,ky,kx;
/*kyhigh为原点与(Zi, Y2i)所成的n条射线的最小斜率*/
/*kylow为原点与(Zi, Y1i)所成的n条射线的最大斜率*/
/*ky,kx分别为子弹射出方向在YOZ和XOZ平面上投影的斜率*/
double qleft,qright,solu;

void inity() {//处理y坐标
	int i;
	kyhigh=1e+10;kylow=-1.0;
	for (i=0;i<n;i++) {
		if (double(yhigh[i])/z[i]<kyhigh) kyhigh=double(yhigh[i])/z[i];
		if (double(ylow[i])/z[i]>kylow) kylow=double(ylow[i])/z[i];
	}
	if (kyhigh<kylow-MIN) {cont='f';return;}//无解
	ky=(kyhigh+kylow)/2;//有解
}

void initx() {//已求得枪手位置，现在求子弹方向
	double maxleft=-1.0,minright=1e+15,re;
	int i;
	for (i=0;i<n;i++) {
		re=(xleft[i]-solu)/(sqrt((xleft[i]-solu)*(xleft[i]-solu)+z[i]*z[i]));
		if (re>maxleft) maxleft=re;
		re=(xright[i]-solu)/(sqrt((xright[i]-solu)*(xright[i]-solu)+z[i]*z[i]));
		if (re<minright) minright=re;
	}
	if (maxleft>minright+MIN) {cont='f';return;}
	kx=((maxleft+minright)/2)/(sin(acos((maxleft+minright)/2)));
}

void main() {
	int i,j;
	double na,nb;
	scanf("%d",&n);
	for (i=0;i<n;i++) scanf("%lf%lf%lf%lf%lf",&xleft[i],&ylow[i],&xright[i],&yhigh[i],&z[i]);
	inity();
	if (cont=='f') {printf("UNSOLVABLE\n");return;}
	qleft=-1e+15;qright=1e+15;//初始化解在OX轴的最大存在区间
	for (i=0;i<n-1;i++) for (j=i+1;j<n;j++) {//缩小解的最大存在区间
		na=xleft[i]-(double(z[i])/double(z[j]-z[i]))*(xright[j]-xleft[i]);
		nb=xright[i]-(double(z[i])/double(z[j]-z[i]))*(xleft[j]-xright[i]);
		if (na>qright+MIN || nb<qleft-MIN) {cont='f';goto pa;}
		if (na>qleft) qleft=na;
		if (nb<qright) qright=nb;
	}
pa:	if (cont=='f') {printf("UNSOLVABLE\n");return;}
	solu=(qleft+qright)/2;
	initx();
	if (cont=='f') {printf("UNSOLVABLE\n");return;}
	printf("SOLUTION\n%.6lf\n",solu);//打印出解
	for (i=0;i<n;i++) printf("%.6lf %.6lf %.6lf\n",solu+z[i]*kx,z[i]*ky,z[i]);
}