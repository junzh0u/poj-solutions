// POJ 1585 - Helping Florida
// Model: gpt-6-astra
// Simulate runoff rounds, eliminating all minimum-vote candidates together.
// Exhausted ballots do not count toward the majority; equal live counts tie.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
int main() {
    int n,b;
    while (cin>>n>>b && (n||b)) {
        string line,s; getline(cin,line);
        map<string,int> ids; vector<string> names;
        vector<vector<int> > ballots(b);
        for(int i=0;i<b;++i) {
            getline(cin,line); istringstream in(line);
            while(in>>s) {
                if(ids.find(s)==ids.end()) { ids[s]=names.size(); names.push_back(s); }
                ballots[i].push_back(ids[s]);
            }
        }
        n=names.size(); vector<int> alive(n,1),pos(b,0);
        while(n) {
            vector<int> votes(n,0); int total=0,lo=b+1,hi=-1,win=-1;
            for(int i=0;i<b;++i) {
                while(pos[i]<(int)ballots[i].size() && !alive[ballots[i][pos[i]]]) ++pos[i];
                if(pos[i]<(int)ballots[i].size()) { ++votes[ballots[i][pos[i]]]; ++total; }
            }
            for(int i=0;i<n;++i) if(alive[i]) {
                lo=min(lo,votes[i]); hi=max(hi,votes[i]);
                if(votes[i]>total/2) win=i;
            }
            if(win>=0) { cout<<names[win]<<" won\n"; break; }
            if(lo==hi) {
                vector<string> tied;
                for(int i=0;i<n;++i) if(alive[i]) tied.push_back(names[i]);
                sort(tied.begin(),tied.end()); cout<<"it is a tie between ";
                for(int i=0;i<(int)tied.size();++i) { if(i) cout<<" and "; cout<<tied[i]; }
                cout<<"\n"; break;
            }
            for(int i=0;i<n;++i) if(alive[i] && votes[i]==lo) alive[i]=0;
        }
    }
}
