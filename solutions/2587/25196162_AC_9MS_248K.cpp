// POJ 2587 - Airline Hub
// Model: gpt-5.6-terra
#include <cstdio>
#include <cmath>
using namespace std;
const double PI=3.14159265358979323846;
int main(){int n,i,best=0; double la[1000],lo[1000],ans=1e100; if(scanf("%d",&n)!=1)return 0; for(i=0;i<n;++i){scanf("%lf%lf",&la[i],&lo[i]);if(la[i]*la[i]+lo[i]*lo[i]<ans){ans=la[i]*la[i]+lo[i]*lo[i];best=i;}} printf("%.2f %.2f\n",la[best],lo[best]);}
