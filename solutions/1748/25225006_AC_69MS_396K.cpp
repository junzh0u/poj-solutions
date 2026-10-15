// POJ 1748 - Computer Dialogue
// Model: gpt-6.1-sol
// Each ignorance message removes files unique in the speaker's current
// information class. Recount before each simultaneous elimination round.
// The extension holder speaks first; absent and empty extensions are equal,
// while original spelling is preserved for output.
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;
int main() {
    int n,m;
    if (!(cin>>n>>m)) return 0;
    vector<string> file(n), name(n), ext(n);
    vector<int> alive(n,1);
    for(int i=0;i<n;++i) {
        cin>>file[i];
        string::size_type p=file[i].find('.');
        name[i]=file[i].substr(0,p);
        ext[i]=(p==string::npos ? "" : file[i].substr(p+1));
    }
    for(int turn=0;turn<m;++turn) {
        map<string,int> count;
        for(int i=0;i<n;++i) if(alive[i])
            ++count[turn%2==0 ? ext[i] : name[i]];
        for(int i=0;i<n;++i) if(alive[i] &&
            count[turn%2==0 ? ext[i] : name[i]]==1) alive[i]=0;
    }
    int total=0;
    for(int i=0;i<n;++i) total+=alive[i];
    cout<<total<<'\n';
    for(int i=0;i<n;++i) if(alive[i]) cout<<file[i]<<'\n';
    return 0;
}
