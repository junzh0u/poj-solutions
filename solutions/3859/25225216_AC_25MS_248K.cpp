// POJ 3859 - ACM Coalition
// Model: gpt-6.1-sol
// Keep the maximum recruited seats for each exact allocation of board roles.
// Each party is skipped or takes one demand; separate layers prevent reuse.
// The discussion clarifies that an impossible coalition outputs 0 0 0.
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
struct Demand { int a,b,c; };
int main() {
    int n; long long m;
    while (cin >> n >> m && (n || m)) {
        string line; getline(cin,line);
        long long dp[2][3][7], nd[2][3][7];
        for(int a=0;a<2;++a) for(int b=0;b<3;++b) for(int c=0;c<7;++c) dp[a][b][c]=-1;
        dp[0][0][0]=0;
        for(int i=0;i<n;++i) {
            getline(cin,line);
            long long seats; istringstream in(line); in >> seats;
            vector<Demand> demands;
            for(string::size_type p=line.find('(');p!=string::npos;p=line.find('(',p+1)) {
                string t=line.substr(p+1,line.find(')',p)-p-1);
                replace(t.begin(),t.end(),',',' ');
                istringstream q(t); Demand d; q>>d.a>>d.b>>d.c; demands.push_back(d);
            }
            for(int a=0;a<2;++a) for(int b=0;b<3;++b) for(int c=0;c<7;++c) nd[a][b][c]=dp[a][b][c];
            for(int a=0;a<2;++a) for(int b=0;b<3;++b) for(int c=0;c<7;++c) if(dp[a][b][c]>=0) {
                for(unsigned j=0;j<demands.size();++j) {
                    Demand d=demands[j];
                    if(a+d.a<=1 && b+d.b<=2 && c+d.c<=6) {
                        long long v=dp[a][b][c]+min(seats,m-dp[a][b][c]);
                        long long &x=nd[a+d.a][b+d.b][c+d.c];
                        if(v>x) x=v;
                    }
                }
            }
            for(int a=0;a<2;++a) for(int b=0;b<3;++b) for(int c=0;c<7;++c) dp[a][b][c]=nd[a][b][c];
        }
        int best=-1, aa=0,bb=0,cc=0;
        for(int a=0;a<2;++a) for(int b=0;b<3;++b) for(int c=0;c<7;++c) if(dp[a][b][c]>=m) {
            int v=25*(1-a)+8*(2-b)+6-c;
            if(v>best) {best=v;aa=1-a;bb=2-b;cc=6-c;}
        }
        cout<<aa<<' '<<bb<<' '<<cc<<'\n';
    }
    return 0;
}
