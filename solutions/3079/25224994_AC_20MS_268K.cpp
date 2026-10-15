// POJ 3079 - Enigmatologically Cruciverbalistic
// Model: gpt-6.1-sol
// Extract maximal horizontal and vertical runs of length at least two.
// Assign unused words with minimum-remaining-values backtracking and forward
// checking. Words read left to right or top to bottom, never in reverse.
#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Slot { vector<int> cells; };
vector<Slot> slots;
vector<string> words;
string board;
bool used[101], assigned[226];
bool fits(int s,int w) {
    if(slots[s].cells.size()!=words[w].size()) return false;
    for(int k=0;k<(int)words[w].size();++k) {
        char ch=board[slots[s].cells[k]];
        if(ch!='#' && ch!=words[w][k]) return false;
    }
    return true;
}
bool solve(int left) {
    if(!left) return board.find('#')==string::npos;
    int best=-1, degree=-1;
    vector<int> choices;
    for(int s=0;s<(int)slots.size();++s) if(!assigned[s]) {
        vector<int> cur;
        for(int w=0;w<(int)words.size();++w) if(!used[w] && fits(s,w)) cur.push_back(w);
        if(cur.empty()) return false;
        int d=0;
        for(int k=0;k<(int)slots[s].cells.size();++k) if(board[slots[s].cells[k]]!='#') ++d;
        if(best<0 || cur.size()<choices.size() || (cur.size()==choices.size() && d>degree)) {
            best=s; choices=cur; degree=d;
        }
    }
    assigned[best]=true;
    for(int q=0;q<(int)choices.size();++q) {
        int w=choices[q];
        vector<int> changed;
        for(int k=0;k<(int)words[w].size();++k) {
            int p=slots[best].cells[k];
            if(board[p]=='#') { changed.push_back(p); board[p]=words[w][k]; }
        }
        used[w]=true;
        if(solve(left-1)) return true;
        used[w]=false;
        for(int k=0;k<(int)changed.size();++k) board[changed[k]]='#';
    }
    assigned[best]=false;
    return false;
}
int main() {
    int tests; cin>>tests;
    for(int tc=1;tc<=tests;++tc) {
        int w,h,c; cin>>w>>h;
        board=""; string row;
        for(int r=0;r<h;++r) { cin>>row; board+=row; }
        slots.clear();
        for(int r=0;r<h;++r) for(int col=0;col<w;++col) if(board[r*w+col]=='#') {
            if(col==0 || board[r*w+col-1]=='.') {
                Slot s;
                for(int j=col;j<w && board[r*w+j]=='#';++j) s.cells.push_back(r*w+j);
                if(s.cells.size()>1) slots.push_back(s);
            }
            if(r==0 || board[(r-1)*w+col]=='.') {
                Slot s;
                for(int i=r;i<h && board[i*w+col]=='#';++i) s.cells.push_back(i*w+col);
                if(s.cells.size()>1) slots.push_back(s);
            }
        }
        cin>>c; words.resize(c);
        for(int i=0;i<c;++i) { cin>>words[i]; used[i]=false; }
        for(int i=0;i<226;++i) assigned[i]=false;
        cout<<"Puzzle #"<<tc<<"\n";
        if((int)slots.size()!=c || !solve(c)) cout<<"I cannot generate this puzzle.\n";
        else for(int r=0;r<h;++r) cout<<board.substr(r*w,w)<<"\n";
    }
}
