// POJ 1910 - Adam's Genes
// Model: gpt-6.1-sol
// Process requests online; invalid clones cannot serve as parents.
// Persistent binary gene sets share unchanged ancestry between clones.
// The first repeated gene is chosen in the request's original order.
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
using namespace std;
struct Node { int child[2]; Node() { child[0]=child[1]=0; } };
class GeneSet {
    vector<Node*> blocks;
    int count;
    Node &node(int x) { return blocks[x/4096][x%4096]; }
    int copy(int x) {
        int y=count++;
        if (y%4096==0) blocks.push_back(new Node[4096]);
        node(y)=node(x);
        return y;
    }
public:
    GeneSet():count(1) { blocks.push_back(new Node[4096]); }
    ~GeneSet() { for(size_t i=0;i<blocks.size();++i) delete[] blocks[i]; }
    bool contains(int root,int gene) {
        for(int bit=14;bit>=0 && root;--bit) root=node(root).child[(gene>>bit)&1];
        return root!=0;
    }
    int insert(int root,int gene,int bit=14) {
        int result=copy(root);
        if(bit>=0) {
            int side=(gene>>bit)&1;
            int child=insert(node(root).child[side],gene,bit-1);
            node(result).child[side]=child;
        }
        return result;
    }
};
int number(const string &s) { return ((s[0]-'A')*26+s[1]-'A')*26+s[2]-'A'; }
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    GeneSet sets;
    map<string,int> roots;
    roots["ADAM"]=0;
    string line;
    while(getline(cin,line)) {
        if(line.empty() || line=="\r") continue;
        istringstream in(line);
        string word,name,parent,gene,bad;
        in>>word>>name>>word>>parent;
        vector<string> mutations;
        if(in>>word) while(in>>gene) mutations.push_back(gene);
        map<string,int>::iterator it=roots.find(parent);
        int root=(it==roots.end() ? -1 : it->second);
        cout<<"clone "<<name<<" from "<<parent;
        if(root<0) cout<<" has no connection to ADAM\n";
        else {
            for(size_t i=0;i<mutations.size();++i)
                if(sets.contains(root,number(mutations[i]))) { bad=mutations[i]; break; }
            if(!bad.empty()) {
                cout<<" was at least twice mutated with "<<bad<<'\n';
                root=-1;
            } else {
                cout<<" is consistent and safe\n";
                for(size_t i=0;i<mutations.size();++i) root=sets.insert(root,number(mutations[i]));
            }
        }
        roots[name]=root;
    }
}
