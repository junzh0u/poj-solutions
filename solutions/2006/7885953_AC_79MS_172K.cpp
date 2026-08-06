#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>

int main(){
	while(true){
		double ka,acid;
		int m,n;
		scanf("%le%le%d%d",&ka,&acid,&m,&n);
		if(m==0 && n==0)
			return 0;
		double a=m*n,b=ka,c=-ka*acid;
		double disolve=(-b+sqrt(b*b-4.0*a*c))/(2.0*a);
		double h=disolve*m;
		double ph=-log(h)/log(10.0);
		printf("%.3f\n",ph);
	}
}