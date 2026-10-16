// POJ 3799 - Optimal Strategy for the ICPC
// Model: gpt-6.1-sol
// Replace an unselected longer job by a shorter one to improve feasibility
// and penalty. Thus only prefixes sorted by (duration, label) are needed.
// Enumerate assignments to three androids; shortest processing time first
// minimizes each android's completion sum. Compare chronological submission
// strings, ordering simultaneous submissions alphabetically, to break ties.
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
using namespace std;
pair<int,int> job[16];
pair<int,int> eventTime[16];
int loadTime[3], n, best;
string answer;
void search(int p,int score) {
    if(score>best) return;
    if(p==n) {
        pair<int,int> e[16];
        for(int i=0;i<n;++i) e[i]=eventTime[i];
        sort(e,e+n);
        string s;
        for(int i=0;i<n;++i) s+=char('A'+e[i].second);
        if(score<best || (score==best && (answer.empty() || s<answer))) {
            best=score; answer=s;
        }
        return;
    }
    for(int b=0;b<3;++b) {
        bool same=false;
        for(int c=0;c<b;++c) if(loadTime[c]==loadTime[b]) same=true;
        if(same || loadTime[b]+job[p].first>300) continue;
        loadTime[b]+=job[p].first;
        eventTime[p]=make_pair(loadTime[b],job[p].second);
        search(p+1,score+loadTime[b]);
        loadTime[b]-=job[p].first;
    }
}
int main() {
    int cases; cin>>cases;
    for(int tc=1;tc<=cases;++tc) {
        int k; cin>>k;
        for(int i=0;i<k;++i) {cin>>job[i].first; job[i].second=i;}
        sort(job,job+k);
        for(n=k;n>=0;--n) {
            best=1000000; answer="";
            loadTime[0]=loadTime[1]=loadTime[2]=0;
            search(0,0);
            if(best<1000000) break;
        }
        cout<<"Data set "<<tc<<":";
        for(int i=0;i<n;++i) cout<<' '<<answer[i];
        cout<<' '<<n<<' '<<best<<'\n';
    }
    return 0;
}
