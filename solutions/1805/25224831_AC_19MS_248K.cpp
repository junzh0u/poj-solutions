// POJ 1805 - Friends
// Model: gpt-6.1-sol
// Collect every applicable preference as a bit; conflicting preferences abstain.
// Match complete names: other people may attend but have no voting rules.
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main() {
    const string names[7] = {"Anne", "Bob", "Karin", "Charly", "Dave", "Edward", "Frank"};
    const char* places[3] = {"cinema", "cocktail bar", "disco"};
    int n; cin >> n;
    string line; getline(cin, line);
    for (int tc=1; tc<=n; ++tc) {
        getline(cin, line);
        bool p[7]={false,false,false,false,false,false,false};
        istringstream in(line); string s;
        while (in >> s) for (int i=0;i<7;++i) if(s==names[i]) p[i]=true;
        int v[7]={0,0,0,0,0,0,0};
        v[0]|=1;
        if(p[2]) v[1]|=4;
        v[2]|=p[3]?4:(p[0]?1:2);
        if(p[4]) v[1]|=2;
        if(p[3] && p[0]) {v[3]|=1; v[0]|=1;}
        v[5]|=(p[0] && !p[3])?2:1;
        if(p[5]) v[1]|=2;
        if(!p[1] && !p[0]) v[6]|=1;
        if(p[0]) v[6]|=4;
        if(!p[0]) v[1]|=2;
        int counts[3]={0,0,0};
        for(int i=0;i<7;++i) if(p[i]) for(int j=0;j<3;++j) if(v[i]==(1<<j)) ++counts[j];
        int best=-1;
        for(int j=0;j<3;++j) if(counts[j]>counts[(j+1)%3] && counts[j]>counts[(j+2)%3]) best=j;
        cout << "Scenario #" << tc << ":\n" << (best<0?"stay at the Hacienda":places[best]) << "\n\n";
    }
    return 0;
}
