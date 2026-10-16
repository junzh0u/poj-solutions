// POJ 1883 - Theseus and the Minotaur
// Model: gpt-6.1-sol
// Simulate simultaneous cavern arrivals and ordered, directed exit marks.
// Theseus scans counterclockwise, following the first Minotaur-marked exit
// after meeting its trail; a candle makes the Minotaur return in one turn.
#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
using namespace std;
string adj[26];
bool tmark[26][26], mm[26][26], candle[26], visited[26];
int choose(int at, int from, int direction, bool marks[26][26], bool follow) {
    int n = adj[at].size(), p = adj[at].find(char('A'+from));
    for (int k=1; k<=n; ++k) {
        int v = adj[at][(p+direction*k+n*n)%n]-'A';
        if (follow ? mm[at][v] : !marks[at][v]) return v;
    }
    return at;
}
void solve(const string &s) {
    memset(tmark,0,sizeof(tmark)); memset(mm,0,sizeof(mm));
    memset(candle,0,sizeof(candle)); memset(visited,0,sizeof(visited));
    int tp=s[0]-'A', t=s[1]-'A', mp=s[2]-'A', m=s[3]-'A';
    while (true) {
        if (t==m) { cout << "The Minotaur is slain in " << char('A'+t) << '\n'; return; }
        if (visited[t]) candle[t]=true;
        visited[m]=true;
        int nt=choose(t,tp,1,tmark,visited[t]);
        int nm=choose(m,mp,-1,mm,false);
        tmark[t][nt]=true; mm[m][nm]=true;
        if (nt==m && nm==t) {
            cout << "Theseus is killed between " << char('A'+t) << " and " << char('A'+nt) << '\n'; return;
        }
        tp=t; t=nt;
        if (candle[nm]) mp=nm;
        else { mp=m; m=nm; }
    }
}
int main() {
    string line;
    while (getline(cin,line)) {
        if (line.empty()) continue;
        if (line[0]=='#') break;
        string letters;
        for (size_t i=0;i<line.size();++i) if (line[i]>='A' && line[i]<='Z') letters+=line[i];
        if (line[0]=='@') { solve(letters); for(int i=0;i<26;++i) adj[i].clear(); }
        else if (!letters.empty()) adj[letters[0]-'A']=letters.substr(1);
    }
    return 0;
}
