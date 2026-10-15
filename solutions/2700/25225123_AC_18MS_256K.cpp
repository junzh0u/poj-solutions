// POJ 2700 - Space Travel
// Model: gpt-6.1-sol
// Reduce the congruence to x=(p+q*k) mod m, y=baseY+g*k.
// Floor sums test whether a total-press budget is feasible; binary search
// finds the minimum budget, then its smallest y (hence largest x).
// The tie rule minimizes x+y first and maximizes x only among those ties.
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long I;
I gcd(I a,I b){return b?gcd(b,a%b):a;}
I egcd(I a,I b,I &x,I &y){if(!b){x=1;y=0;return a;} I u,v;I d=egcd(b,a%b,u,v);x=v;y=u-(a/b)*v;return d;}
I mod(I a,I m){a%=m;return a<0?a+m:a;}
I inv(I a,I m){if(m==1)return 0;I x,y;egcd(a,m,x,y);return mod(x,m);}
// Sum floor((a*i+b)/m), for 0<=i<n, including negative b.
I floors(I n,I m,I a,I b){
    I ans=0;
    if(b<0){I z=(-b+m-1)/m;b+=z*m;ans-=n*z;}
    while(true){
        if(a>=m){ans+=(n*(n-1)/2)*(a/m);a%=m;}
        if(b>=m){ans+=n*(b/m);b%=m;}
        I z=a*n+b;if(z<m)return ans;
        n=z/m;b=z%m;swap(m,a);
    }
}
I p,q,g,m,baseY;
I count(I budget,I last){
    I z=budget-baseY;if(z<0)return 0;
    I k=min(last,min(m-1,z/g));if(k<0)return 0;
    return floors(k+1,m,q,p)-floors(k+1,m,q+g,p-z-1);
}
int main(){
    I n,a,b,s,t;
    while(cin>>n && n){
        cin>>a>>b>>s>>t;
        if(s<0||t<0||s>=n||t>=n){cout<<"no solution\n";continue;}
        a=mod(a,n);b=mod(b,n);I c=mod(t-s,n);
        I d=gcd(gcd(a,b),n);
        if(c%d){cout<<"no solution\n";continue;}
        n/=d;a/=d;b/=d;c/=d;
        g=gcd(a,n);m=n/g;
        baseY=mod(c*inv(b,g),g);
        I ai=inv(a/g,m);
        p=mod(((c-b*baseY)/g)*ai,m);q=mod(-b*ai,m);
        I lo=0,hi=n-1;
        while(lo<hi){I mid=(lo+hi)/2;if(count(mid,m-1)>0)hi=mid;else lo=mid+1;}
        I best=lo;lo=0;hi=min(m-1,(best-baseY)/g);
        while(lo<hi){I mid=(lo+hi)/2;if(count(best,mid)>0)hi=mid;else lo=mid+1;}
        I y=baseY+g*lo;cout<<best-y<<' '<<y<<'\n';
    }
}
