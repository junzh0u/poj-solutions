// POJ 2061 - Pseudo-random Numbers
// Model: gpt-6.1-sol
// Reconstruct a forced seed suffix backwards, then propagate known digits.
// Digits are stored least significant first; a carry consumes a following 1.
// Truncated simulations are retried with a larger suffix if information runs out.
#include <iostream>
#include <vector>
using namespace std;
int main() {
    int cases; cin >> cases;
    while (cases--) {
        int b,l,t; cin >> b >> l;
        vector<int> out(l); int i;
        for(i=0;i<l;++i) cin >> out[i];
        cin >> t;
        vector<int> a(1,out[l-1]); bool ok=true;
        for(int k=l-2;k>=0 && ok;--k) {
            vector<int> p(1,out[k]);
            for(i=0;i<(int)a.size();++i) {
                int x=p.back(), y=a[i];
                p.push_back((y-x+b)%b);
                if(y<x) {
                    ++i;
                    if(i<(int)a.size() && a[i]!=1) {ok=false;break;}
                }
            }
            a=p;
        }
        if(!ok) {cout << "impossible\n";continue;}
        int cap=128, answer=-1;
        for(;;cap*=2) {
            vector<int> cur=a; bool cut=false, retry=false;
            for(int k=1;k<t;++k) {
                if(cur.size()<2) {retry=cut;break;}
                vector<int> next;
                for(i=0;i+1<(int)cur.size();++i) {
                    int s=cur[i]+cur[i+1];
                    next.push_back(s%b);
                    if(s>=b) next.push_back(1);
                    if((int)next.size()>=cap) {next.resize(cap);cut=true;break;}
                }
                cur.swap(next);
                if(k==t-1) answer=cur[0];
            }
            if(!retry) break;
        }
        if(answer<0) cout << "unpredictable\n";
        else cout << answer << '\n';
    }
}
