// POJ 4040 - Non-negative Partial Sums
// Model: gpt-6.1-sol
// Count starts whose suffix and wrapped prefix never fall below the start.
// Prefix sums and suffix minima give both tests in linear time and space.
// POJ's statement is empty; SWERC 2011 / UVa 12393 supplies the statement.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
class Input {
    char b[65536]; int p,n;
public:
    Input():p(0),n(0){}
    int get(){if(p==n){n=(int)fread(b,1,sizeof(b),stdin);p=0;if(!n)return EOF;}return b[p++];}
    bool read(int &v){int c; do{c=get();if(c==EOF)return false;}while(c<=32);int s=1;if(c=='-'){s=-1;c=get();}v=0;while(c>32&&c!=EOF){v=v*10+c-'0';c=get();}v*=s;return true;}
};
int main(){
    Input in; int n;
    while(in.read(n)&&n){
        vector<int> p(n+1), low(n+1);
        for(int i=1;i<=n;++i){int x;in.read(x);p[i]=p[i-1]+x;}
        low[n]=p[n];
        for(int i=n-1;i>=0;--i)low[i]=min(p[i],low[i+1]);
        int prefix=0,answer=0;
        for(int k=0;k<n;++k){
            prefix=min(prefix,p[k]);
            if(low[k+1]>=p[k] && p[n]+prefix>=p[k])++answer;
        }
        printf("%d\n",answer);
    }
    return 0;
}
