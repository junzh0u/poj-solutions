// POJ 3403 - "Bloody Mary"
// Model: gpt-6-astra
// Find a nonnegative combination using one, two, or three independent
// composition vectors. Cramer's rule keeps all arithmetic exact.
// Coefficients multiply unnormalized compositions; multiply each by its
// component sum to recover the requested liquid-volume ratio.
#include <iostream>
using namespace std;
typedef long long Int;
Int a[4][3], s[3];
Int gcd(Int x, Int y) { return y ? gcd(y,x%y) : x; }
Int det(const Int *x,const Int *y,const Int *z) {
    return x[0]*(y[1]*z[2]-y[2]*z[1])
         - x[1]*(y[0]*z[2]-y[2]*z[0])
         + x[2]*(y[0]*z[1]-y[1]*z[0]);
}
void output(Int x,Int y,Int z) {
    x*=s[0]; y*=s[1]; z*=s[2];
    Int g=gcd(gcd(x,y),z);
    cout<<x/g<<':'<<y/g<<':'<<z/g<<'\n';
}
int main() {
    char colon;
    for(int i=0;i<4;++i) {
        if(!(cin>>a[i][0]>>colon>>a[i][1]>>colon>>a[i][2])) return 0;
        if(i<3) s[i]=a[i][0]+a[i][1]+a[i][2];
    }
    for(int i=0;i<3;++i) {
        bool same=true;
        for(int p=0;p<3;++p) for(int q=p+1;q<3;++q)
            if(a[i][p]*a[3][q]!=a[i][q]*a[3][p]) same=false;
        if(same) { Int x[3]={0,0,0}; x[i]=1; output(x[0],x[1],x[2]); return 0; }
    }
    for(int i=0;i<3;++i) for(int j=i+1;j<3;++j)
        for(int p=0;p<3;++p) for(int q=p+1;q<3;++q) {
            Int d=a[i][p]*a[j][q]-a[i][q]*a[j][p];
            if(!d) continue;
            Int u=a[3][p]*a[j][q]-a[3][q]*a[j][p];
            Int v=a[i][p]*a[3][q]-a[i][q]*a[3][p];
            if(d<0) {d=-d;u=-u;v=-v;}
            if(u<0 || v<0) continue;
            bool ok=true;
            for(int k=0;k<3;++k) if(u*a[i][k]+v*a[j][k]!=d*a[3][k]) ok=false;
            if(ok) {Int x[3]={0,0,0};x[i]=u;x[j]=v;output(x[0],x[1],x[2]);return 0;}
        }
    Int d=det(a[0],a[1],a[2]);
    if(d) {
        Int x=det(a[3],a[1],a[2]),y=det(a[0],a[3],a[2]),z=det(a[0],a[1],a[3]);
        if(d<0) {x=-x;y=-y;z=-z;}
        if(x>=0 && y>=0 && z>=0) {output(x,y,z);return 0;}
    }
    cout<<"impossible\n";
}
