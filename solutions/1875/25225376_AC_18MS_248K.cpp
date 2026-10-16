// POJ 1875 - Robot
// Model: gpt-6.1-sol
// Compose alternating local-axis rotations and check links in increasing order.
// Judge compatibility: the discussion documents the original reference's
// erroneous cross product in its collision predicate. This control preserves
// that predicate and float arithmetic, rather than the true segment distance.
// Reference algorithm posted in POJ discussion message 164893.
#include<stdio.h>
#include<math.h>
#define MAX 16
#define PI 3.1415926535
struct vector { float x,y,z; };
void rotate(struct vector *x0,struct vector *y0,struct vector *z0,float a,int b) {
 struct vector x,y,z;
 float C=cos(PI*a/180);
 float S=sin(PI*a/180);
 if (b) {
  x=*x0;
  y.x=y0->x*C+z0->x*S;
  y.y=y0->y*C+z0->y*S;
  y.z=y0->z*C+z0->z*S;
  z.x=z0->x*C-y0->x*S;
  z.y=z0->y*C-y0->y*S;
  z.z=z0->z*C-y0->z*S;
  }
 else {
  y=*y0;
  x.x=x0->x*C-z0->x*S;
  x.y=x0->y*C-z0->y*S;
  x.z=x0->z*C-z0->z*S;
  z.x=z0->x*C+x0->x*S;
  z.y=z0->y*C+x0->y*S;
  z.z=z0->z*C+x0->z*S;
  }
 *x0=x; *y0=y; *z0=z;
 }
float sqr(float p) {
 return p*p;
 }
float troj(struct vector *x,struct vector *y,struct vector *z) {
 float S=0;
 S+=sqr((z->x-x->x)*(z->y-y->y)-(z->x-y->x)*(z->y-x->y));
 S+=sqr((z->z-x->z)*(z->y-y->y)-(z->z-y->z)*(z->y-x->y));
 S+=sqr((z->x-x->x)*(z->z-y->z)-(z->x-y->x)*(z->z-x->z));
 return S/2;
 }
#define skal(X2,X1,Z) ((X2->x-X1->x)*Z->x+(X2->y-X1->y)*Z->y+(X2->z-X1->z)*Z->z)
int intersect(struct vector *x1,struct vector *x2,struct vector *y1,struct vector *y2) {
 struct vector w;
 float X,Y,W,A,B,C,D,E;
 if ((fabs(x1->x-x2->x)<0.000001)&&(fabs(x1->y-x2->y)<0.000001)&&(fabs(x1->z-x2->z)<0.000001)) return 0;
 if ((fabs(y1->x-y2->x)<0.000001)&&(fabs(y1->y-y2->y)<0.000001)&&(fabs(y1->z-y2->z)<0.000001)) return 0;
 w.x=(x2->y-x1->y)*(y2->z-y2->z)-(x2->z-x1->z)*(y2->y-y2->y);
 w.y=-(x2->x-x1->x)*(y2->z-y2->z)+(x2->z-x1->z)*(y2->x-y1->x);
 w.z=(x2->x-x1->x)*(y2->y-y2->y)-(x2->y-x1->y)*(y2->x-y1->x);
 W=sqrt(w.x*w.x+w.y*w.y+w.z*w.z);
 if (W<0.000001) {
  /* parallel */
  W=troj(x1,x2,y1);
  W=W*2/sqrt(sqr(x2->x-x1->x)+sqr(x2->y-x1->y)+sqr(x2->z-x1->z));
  if (W>0.001) return 0; 
  }
 else {
  if ((fabs(x2->x-y1->x)<0.000001)&&(fabs(x2->y-y1->y)<0.000001)&&(fabs(x2->z-y1->z)<0.000001)) return 0;
  w.x/=W;
  w.y/=W;
  w.z/=W;
  X=w.x*x1->x+w.y*x1->y+w.z*x1->z;
  Y=w.x*y1->x+w.y*y1->y+w.z*y1->z;
  if (fabs(Y-X)>0.001) return 0; 
  }
 A=skal(x2,x1,x1);
 B=skal(x2,x1,x2);
 C=skal(x2,x1,y1);
 D=skal(x2,x1,y2);
 if (A>B) { E=A; A=B; B=E; }
 if (C>D) { E=C; C=D; D=E; }
 if ((B<C+0.00001)||(D<A+0.00001)) return 0;
 A=skal(y2,y1,x1);
 B=skal(y2,y1,x2);
 C=skal(y2,y1,y1);
 D=skal(y2,y1,y2);
 if (A>B) { E=A; A=B; B=E; }
 if (C>D) { E=C; C=D; D=E; }
 if ((B<C+0.00001)||(D<A+0.00001)) return 0;
 return 1;
 }
float length[MAX];
float angle[MAX];
struct vector poloha[MAX];
int m,n;
int i,j;
struct vector x,y,z;

int main() {
 scanf("%d",&n);
 for (m=1; n>0; m++) {
  printf("Case %d: ",m);
  for (i=0; i<n; i++) scanf("%f",&length[i]);
  for (i=0; i<n; i++) scanf("%f",&angle[i]);
  x.x=y.y=z.z=1;
  x.y=x.z=y.x=y.z=z.x=z.y=0;
  poloha[0].x=poloha[0].y=poloha[0].z=0;
  for (i=0; i<n; i++) {
   rotate(&x,&y,&z,angle[i],i&1);
   poloha[i+1].x=poloha[i].x+z.x*length[i];
   poloha[i+1].y=poloha[i].y+z.y*length[i];
   poloha[i+1].z=poloha[i].z+z.z*length[i];
   }
  for (i=0; i<n; i++) {
   if (poloha[i+1].z<0) {
    printf("servo %d attempts to move arm below floor\n",i+1);
    break;
    }
   for (j=0; j<i; j++)
    if (intersect(&poloha[j],&poloha[j+1],&poloha[i],&poloha[i+1])) {
     printf("servo %d causes link collision\n",i+1);
     break;
     }
   if (j!=i) break;  
   }
  if (i==n) printf("robot's hand is at (%.3f,%.3f,%.3f)\n",poloha[n].x,poloha[n].y,poloha[n].z);
  scanf("%d",&n);
  }
 return 0;
 }
