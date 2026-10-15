// POJ 1779 - Boolean Logic
// Model: gpt-6.1-sol
// Parse a postorder syntax tree, retaining original character positions.
// Three-character operators are centered on their second non-space character.
// Enumerate assignments in alphabetic lexicographic order and evaluate all nodes.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Node { char op; int pos, left, right; Node(char o,int p,int l,int r):op(o),pos(p),left(l),right(r){} };
string line;
vector<Node> nodes;
int at;
void spaces() { while(at<(int)line.size() && line[at]==' ') ++at; }
int parse() {
    spaces();
    if(line[at]>='a' && line[at]<='z') {
        int p=at++; nodes.push_back(Node(line[p],p,-1,-1));
        return (int)nodes.size()-1;
    }
    ++at; spaces();
    if(line[at]=='!') {
        int p=at++; int a=parse(); spaces(); ++at;
        nodes.push_back(Node('!',p,a,-1));
    } else {
        int a=parse(); spaces(); char op=line[at]; int p=at++;
        if(op=='-' || op=='<') {
            spaces(); p=at++; spaces(); ++at;
        }
        int b=parse(); spaces(); ++at;
        nodes.push_back(Node(op,p,a,b));
    }
    return (int)nodes.size()-1;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line.find_first_not_of(' ')==string::npos) continue;
        nodes.clear(); at=0; parse();
        bool used[26]={false}; int rank[26],n=0;
        for(int i=0;i<(int)nodes.size();++i)
            if(nodes[i].op>='a' && nodes[i].op<='z') used[nodes[i].op-'a']=true;
        for(int i=0;i<26;++i) if(used[i]) rank[i]=n++;
        cout<<line<<'\n';
        vector<int> value(nodes.size());
        for(unsigned mask=0; mask<(1u<<n); ++mask) {
            string out(line.size(),' ');
            for(int i=0;i<(int)nodes.size();++i) {
                Node &t=nodes[i]; int v;
                if(t.op>='a' && t.op<='z') v=(mask>>(n-1-rank[t.op-'a']))&1;
                else if(t.op=='!') v=!value[t.left];
                else {
                    int a=value[t.left],b=value[t.right];
                    if(t.op=='&') v=a&b;
                    else if(t.op=='|') v=a|b;
                    else if(t.op=='-') v=(!a)|b;
                    else v=(a==b);
                }
                value[i]=v; out[t.pos]=char('0'+v);
            }
            cout<<out<<'\n';
        }
        cout<<'\n';
    }
}
