// POJ 3564 - Magic pyramid
// Model: gpt-6-astra
// Meet in the middle: four inverse moves from the solved coloring and
// five forward moves from the input cover every promised solution.
// Normalize color names by first occurrence; moves commute with renaming.
// Face permutations use the folded diagram, checked against Figure 2.
// Repaint is directed: the backward search uses its inverse permutation.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
int face[36] = {0,0,0,0,0,1,2,2,2,2,2,0,0,0,1,1,1,2,2,2,0,1,1,1,1,1,2,3,3,3,3,3,3,3,3,3};
int rot[4][36] = {
{20,12,11,1,0,10,35,33,32,28,27,13,3,2,8,9,16,17,18,19,4,6,7,23,24,25,26,5,15,29,30,31,14,22,34,21},
{0,1,2,28,27,21,20,12,8,9,10,11,30,29,23,22,14,13,3,19,31,25,24,16,15,5,4,26,18,17,7,6,32,33,34,35},
{5,15,16,24,25,31,26,18,17,7,6,11,12,13,14,30,34,19,9,8,20,21,22,23,33,35,10,27,28,29,1,0,32,3,2,4},
{26,18,2,3,4,5,6,7,8,24,25,19,9,13,14,15,16,17,22,23,10,0,1,11,12,20,21,35,33,32,28,27,34,30,29,31}};
int moveperm[9][36], revperm[9][36];
const string symbols="xXuUvVwW*";
string normalize(string s) {
    int names[256]; fill(names,names+256,-1); int n=0;
    for(int i=0;i<36;++i) {
        unsigned char c=s[i];
        if(names[c]<0) names[c]=n++;
        s[i]=char('a'+names[c]);
    }
    return s;
}
string transformState(const string &s,const int *p) {
    string t(36,'a');
    for(int i=0;i<36;++i) t[i]=s[p[i]];
    return normalize(t);
}
struct Node {string s,path; Node(string a,string b):s(a),path(b) {}};
int main() {
    string start;
    if(!(cin>>start)) return 0;
    for(int f=0;f<4;++f) for(int i=0;i<36;++i) {
        moveperm[2*f][i]=rot[f][i];
        moveperm[2*f+1][rot[f][i]]=i;
    }
    for(int i=0;i<36;++i) {cin>>moveperm[8][i]; --moveperm[8][i];}
    for(int m=0;m<9;++m) for(int i=0;i<36;++i) revperm[m][moveperm[m][i]]=i;
    string goal(36,'a');
    for(int i=0;i<36;++i) goal[i]=char('a'+face[i]);
    goal=normalize(goal); start=normalize(start);
    map<string,string> back;
    vector<Node> q; q.push_back(Node(goal,"")); back[goal]="";
    for(size_t h=0;h<q.size();++h) {
        Node cur=q[h];
        if(cur.path.size()==4) continue;
        for(int m=0;m<9;++m) {
            string s=transformState(cur.s,revperm[m]);
            string path=symbols[m]+cur.path;
            if(back.insert(make_pair(s,path)).second) q.push_back(Node(s,path));
        }
    }
    q.clear(); q.push_back(Node(start,""));
    map<string,int> seen; seen[start]=0;
    string answer(10,'?');
    for(size_t h=0;h<q.size();++h) {
        Node cur=q[h];
        map<string,string>::iterator it=back.find(cur.s);
        if(it!=back.end() && cur.path.size()+it->second.size()<answer.size())
            answer=cur.path+it->second;
        if(cur.path.size()==5 || cur.path.size()>=answer.size()) continue;
        for(int m=0;m<9;++m) {
            string s=transformState(cur.s,moveperm[m]);
            if(seen.insert(make_pair(s,0)).second)
                q.push_back(Node(s,cur.path+symbols[m]));
        }
    }
    cout<<answer<<'\n';
}
