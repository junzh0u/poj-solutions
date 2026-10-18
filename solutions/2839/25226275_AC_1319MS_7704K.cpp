// POJ 2839 - Convex Hull and Triangle
// Model: gpt-6-astra
// Sum oriented boundary integrals of hull edges inside the triangle and
// triangle edges inside the hull. Binary searches locate each line's two
// hull crossings; prefix sums aggregate intact hull chains in O(log n).
// Collinear shared edges are counted only as hull edges. Input is one case.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
// Double-double arithmetic keeps the final integer rounding independent of
// whether the compiler provides an extended-precision long double type.
struct R {
    double hi, lo;
    R(double x=0): hi(x), lo(0) {}
    R(double h, double l): hi(h), lo(l) {}
};
R operator+(R a, R b) {
    double s=a.hi+b.hi, v=s-a.hi;
    double e=(a.hi-(s-v))+(b.hi-v)+a.lo+b.lo;
    double h=s+e;
    return R(h,e-(h-s));
}
R operator-(R a) { return R(-a.hi,-a.lo); }
R operator-(R a, R b) { return a+R(-b.hi,-b.lo); }
R operator*(R a, R b) {
    const double split=134217729.0;
    double p=a.hi*b.hi;
    double c=split*a.hi, ah=c-(c-a.hi), al=a.hi-ah;
    c=split*b.hi; double bh=c-(c-b.hi), bl=b.hi-bh;
    double e=((ah*bh-p)+ah*bl+al*bh)+al*bl;
    e+=a.hi*b.lo+a.lo*b.hi+a.lo*b.lo;
    double h=p+e;
    return R(h,e-(h-p));
}
R operator/(R a, R b) {
    double q=a.hi/b.hi;
    R rem=a-b*R(q);
    return R(q)+R((rem.hi+rem.lo)/b.hi);
}
bool operator<(R a,R b) {return a.hi!=b.hi?a.hi<b.hi:a.lo<b.lo;}
bool operator>(R a,R b) {return b<a;}
bool operator<=(R a,R b) {return !(b<a);}
bool operator>=(R a,R b) {return !(a<b);}
bool operator==(R a,R b) {return a.hi==b.hi&&a.lo==b.lo;}
bool operator!=(R a,R b) {return !(a==b);}
R& operator+=(R& a,R b) {a=a+b;return a;}
R& operator-=(R& a,R b) {a=a-b;return a;}
R atan2l(R y,R x) {return R(atan2(y.hi,x.hi));}
R acosl(R x) {return R(acos(x.hi));}
double rounded(R a) {
    a+=R(0.5);
    a+=R(1e-12); // Resolve exact half-integer ties after arithmetic roundoff.
    double f=floor(a.hi);
    return f+floor((a.hi-f)+a.lo);
}
struct P { R x,y; P(R X=0,R Y=0):x(X),y(Y){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(R t)const{return P(x*t,y*t);} bool operator<(P b)const{return x!=b.x?x<b.x:y<b.y;} bool operator==(P b)const{return x==b.x&&y==b.y;} };
R cross(P a,P b){return a.x*b.y-a.y*b.x;}
R dot(P a,P b){return a.x*b.x+a.y*b.y;}
vector<P> h; vector<R> pref; vector<pair<R,int> > ang; int H;
R side(P a,P d,int i){return cross(d,h[i%H]-a);}
int support(R t){ const R pi=acosl(R(-1)); if(t>pi)t-=2*pi; if(t<-pi)t+=2*pi; int j=lower_bound(ang.begin(),ang.end(),make_pair(t,-1))-ang.begin();return ang[j==H?0:j].second;}
typedef pair<int,int> Range;
vector<Range> intersectRanges(vector<Range> a,vector<Range> b){vector<Range> c;for(unsigned i=0;i<a.size();++i)for(unsigned j=0;j<b.size();++j){int l=max(a[i].first,b[j].first),r=min(a[i].second,b[j].second);if(l<=r)c.push_back(Range(l,r));}return c;}
vector<Range> cyclic(int l,int r){vector<Range> v;if(l<=r)v.push_back(Range(l,r));else{v.push_back(Range(l,H-1));v.push_back(Range(0,r));}return v;}
R solve(P t[3]){
 R tr=cross(t[1]-t[0],t[2]-t[0]); if(H<3||tr==0)return 0; if(tr<0)swap(t[1],t[2]);
 vector<Range> ranges(1,Range(0,H-1)); vector<int> cut; R ans=0;
 for(int s=0;s<3;++s){P a=t[s],d=t[(s+1)%3]-a;R theta=atan2l(d.y,d.x);int mn=support(theta),mx=support(theta+acosl(R(-1)));R lo=side(a,d,mn),hi=side(a,d,mx);
  if(hi<=0)return 0; if(lo>=0)continue;
  int len=(mx-mn+H)%H,L=0,U=len;while(L<U){int mid=(L+U)/2;if(side(a,d,mn+mid)>=0)U=mid;else L=mid+1;}int first=(mn+L)%H;
  len=(mn-mx+H)%H;L=0;U=len;while(L<U){int mid=(L+U+1)/2;if(side(a,d,mx+mid)>=0)L=mid;else U=mid-1;}int last=(mx+L)%H;
  vector<Range> ok;if(first!=last)ok=cyclic(first,(last+H-1)%H);ranges=intersectRanges(ranges,ok);
  int e[2]={(first+H-1)%H,last};P hit[2];for(int k=0;k<2;++k){cut.push_back(e[k]);P p=h[e[k]],q=h[(e[k]+1)%H];R u=cross(d,p-a),v=cross(d,q-a);hit[k]=p+(q-p)*(u/(u-v));}
  R u=dot(hit[0]-a,d)/dot(d,d),v=dot(hit[1]-a,d)/dot(d,d);if(u>v)swap(u,v);u=max(u,(R)0);v=min(v,(R)1);if(u<v)ans+=cross(a+d*u,a+d*v);
 }
 for(unsigned i=0;i<ranges.size();++i)ans+=pref[ranges[i].second+1]-pref[ranges[i].first];
 sort(cut.begin(),cut.end());cut.erase(unique(cut.begin(),cut.end()),cut.end());
 for(unsigned i=0;i<cut.size();++i){int e=cut[i];bool counted=false;for(unsigned j=0;j<ranges.size();++j)if(ranges[j].first<=e&&e<=ranges[j].second)counted=true;if(counted)continue;P a=h[e],b=h[(e+1)%H];R l=0,r=1;for(int s=0;s<3;++s){P d=t[(s+1)%3]-t[s];R u=cross(d,a-t[s]),v=cross(d,b-t[s]);if(u<0&&v<0){r=-1;break;}if(u<0)l=max(l,u/(u-v));if(v<0)r=min(r,u/(u-v));}if(l<r)ans+=cross(a+(b-a)*l,a+(b-a)*r);}
 return max((R)0,ans/2);
}
int main(){int n;if(scanf("%d",&n)!=1)return 0;vector<P> p(n);for(int i=0;i<n;++i){int x,y;scanf("%d%d",&x,&y);p[i]=P(x,y);}sort(p.begin(),p.end());p.erase(unique(p.begin(),p.end()),p.end());n=p.size();for(int i=0;i<n;++i){while(h.size()>1&&cross(h.back()-h[h.size()-2],p[i]-h.back())<=0)h.pop_back();h.push_back(p[i]);}int lower=h.size();for(int i=n-2;i>=0;--i){while((int)h.size()>lower&&cross(h.back()-h[h.size()-2],p[i]-h.back())<=0)h.pop_back();h.push_back(p[i]);}if(h.size()>1)h.pop_back();H=h.size();pref.push_back(0);for(int i=0;i<H;++i){P d=h[(i+1)%H]-h[i];ang.push_back(make_pair(atan2l(d.y,d.x),i));pref.push_back(pref.back()+cross(h[i],h[(i+1)%H]));}sort(ang.begin(),ang.end());int m;scanf("%d",&m);while(m--){P t[3];for(int i=0;i<3;++i){int x,y;scanf("%d%d",&x,&y);t[i]=P(x,y);}R a=solve(t);printf("%.0f\n",rounded(a));}return 0;}
