// POJ 1851 - Map
// Model: gpt-6-astra
// Waterloo 2004-09-19 official A.c reference, with C++98 declarations,
// portable printf formatting and Euclidean norm. Diagonal-first ordering;
// exact coordinate equality controls one versus two instructions.
#include <stdio.h>
#include <string.h>
#include <math.h>

double norm(double a,double b){return sqrt(a*a+b*b);}

#define EPS 1e-10

double r,x,y,xx,yy,X,Y,dx,dy,dd,c;

void move(double x, double y, double X, double Y){
   char tmp[100];
   tmp[0] = 0;
   if (Y - y > EPS) strcpy(tmp,"north");
   if (Y - y < -EPS) strcpy(tmp,"south");
   if (X - x > EPS) strcat(tmp,"east");
   if (X - x < -EPS) strcat(tmp,"west");
   printf("%s %0.10f\n",tmp,norm(X-x,Y-y));
}

int main(){
   while (5 == scanf("%lf%lf%lf%lf%lf",&r,&x,&y,&X,&Y)) {
      if (c++) printf("\n");
      dx = fabs(X-x);
      dy = fabs(Y-y);
      if (dx == 0 || dy == 0 || dx == dy) {
         move(x,y,X,Y);
         continue;
      }
      dd = dx < dy ? dx : dy;
      xx = x + (X>x?dd:-dd);
      yy = y + (Y>y?dd:-dd);
      if (norm(xx,yy) < r) {
         move(x,y,xx,yy);
         move(xx,yy,X,Y);
         continue;
      }
      xx = X - (X>x?dd:-dd);
      yy = Y - (Y>y?dd:-dd);

      move(x,y,xx,yy);
      move(xx,yy,X,Y);
   }
   return 0;
}
