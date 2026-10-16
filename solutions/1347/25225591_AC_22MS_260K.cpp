// POJ 1347 - Triangle
// Model: gpt-6-astra
// Complete a sufficient SSS, ASA/AAS, SAS or SSA subset, then check every
// supplied parameter. SSA may have two solutions; insufficient information
// (infinitely many triangles) is Invalid input, not More than one solution.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0);
double inS[3],inA[3],ansS[3],ansA[3];
int countAns;
bool closeEnough(double x,double y) {return fabs(x-y)<=1e-7*max(1.0,max(fabs(x),fabs(y)));}
void candidate(double *s,double *a) {
    for(int i=0;i<3;++i) {
        if(!(s[i]>0 && a[i]>0 && a[i]<PI)) return;
        if(inS[i]!=-1 && !closeEnough(s[i],inS[i])) return;
        if(inA[i]!=-1 && !closeEnough(a[i],inA[i])) return;
    }
    if(countAns && closeEnough(a[0],ansA[0]) && closeEnough(a[1],ansA[1])) return;
    ++countAns;
    for(int i=0;i<3;++i) {ansS[i]=s[i]; ansA[i]=a[i];}
}
void fromSides(double *s) {
    double a[3];
    for(int i=0;i<3;++i) {
        int j=(i+1)%3,k=(i+2)%3;
        if(s[i]>=s[j]+s[k] || s[i]<=fabs(s[j]-s[k])) return;
        double v=(s[j]*s[j]+s[k]*s[k]-s[i]*s[i])/(2*s[j]*s[k]);
        a[i]=acos(max(-1.0,min(1.0,v)));
    }
    candidate(s,a);
}
int main() {
    int t; scanf("%d",&t);
    while(t--) {
        int ns=0,na=0; bool valid=true;
        double s[3],a[3];
        for(int i=0;i<3;++i) {
            scanf("%lf%lf",&inS[i],&inA[i]); s[i]=inS[i]; a[i]=inA[i];
            if(s[i]!=-1) {++ns; if(s[i]<=0) valid=false;}
            if(a[i]!=-1) {++na; if(a[i]<=0 || a[i]>=PI) valid=false;}
        }
        countAns=0;
        if(valid && ns==3) fromSides(s);
        else if(valid && ns>=1 && na>=2) {
            double sum=0; int missing=-1,knownSide=0;
            for(int i=0;i<3;++i) {if(a[i]==-1) missing=i; else sum+=a[i]; if(s[i]!=-1) knownSide=i;}
            if(missing!=-1) a[missing]=PI-sum;
            if((missing!=-1 || closeEnough(sum,PI)) && a[0]>0 && a[1]>0 && a[2]>0) {
                double scale=s[knownSide]/sin(a[knownSide]);
                for(int i=0;i<3;++i) s[i]=scale*sin(a[i]);
                candidate(s,a);
            }
        } else if(valid && ns==2 && na==1) {
            int missing=0,angle=0;
            for(int i=0;i<3;++i) {if(s[i]==-1) missing=i; if(a[i]!=-1) angle=i;}
            if(angle==missing) {
                int j=(missing+1)%3,k=(missing+2)%3;
                s[missing]=sqrt((s[j]-s[k])*(s[j]-s[k])+4*s[j]*s[k]*sin(a[angle]/2)*sin(a[angle]/2));
                fromSides(s);
            } else {
                int other=3-angle-missing;
                double v=s[other]*sin(a[angle])/s[angle];
                if(v<=1+1e-12) {
                    double first=asin(min(1.0,v));
                    for(int choice=0;choice<2;++choice) {
                        a[other]=choice?PI-first:first;
                        a[missing]=PI-a[angle]-a[other];
                        s[missing]=s[angle]*sin(a[missing])/sin(a[angle]);
                        candidate(s,a);
                    }
                }
            }
        }
        if(!countAns) puts("Invalid input.");
        else if(countAns>1) puts("More than one solution.");
        else {for(int i=0;i<3;++i) printf("%.12f %.12f%c",inS[i]==-1?ansS[i]:inS[i],inA[i]==-1?ansA[i]:inA[i],i==2?'\n':' ');}
    }
}
